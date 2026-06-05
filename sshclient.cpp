#include "sshclient.h"

#ifdef Q_OS_UNIX
#include <netinet/tcp.h>
#endif

#include <QHostInfo>
#define POLL_TIMEOUT 1000
static int waitsocket(int socket_fd, LIBSSH2_SESSION *session) {
  struct timeval timeout;
  int rc;
  fd_set fd;
  fd_set *writefd = NULL;
  fd_set *readfd = NULL;
  int dir;

  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  FD_ZERO(&fd);

  FD_SET(socket_fd, &fd);

  /* now make sure we wait in the correct direction */
  dir = libssh2_session_block_directions(session);

  if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
    readfd = &fd;

  if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
    writefd = &fd;

  rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

  return rc;
}

static int waitsocket(int socket_fd, int timeout_sec) {
  struct timeval timeout;
  int rc;
  fd_set fd;
  timeout.tv_sec = timeout_sec;
  timeout.tv_usec = 0;
  FD_ZERO(&fd);
  FD_SET(socket_fd, &fd);
  rc = select(socket_fd + 1, NULL, &fd, NULL, &timeout);
  return rc;
}

SSHClient::SSHClient(QString hostName, QString port, QString username,
                     const QString password)
    : QThread() {
  this->hostName = hostName;
  QHostInfo info = QHostInfo::fromName(hostName);
  if (info.addresses().isEmpty()) {
    fprintf(stderr, "Failed to resolve hostname\n");
    return;
  }
  QString resolvedHost = info.addresses().first().toString();
  this->hostaddr = inet_addr(resolvedHost.toUtf8().constData());
  this->port = htons(atoi(port.toUtf8().constData()));
  this->username = username;
  this->password = password;
  if (libssh2_init(0) != 0) {
    fprintf(stderr, "libssh2 initialization failed\n");
  }
}

SSHClient::SSHClient(QString hostName, QString port, QString username,
                     QString publicKeyPath, QString privateKeyPath,
                     QString passPhrase)
    : QThread() {
  this->hostName = hostName;
  QHostInfo info = QHostInfo::fromName(hostName);
  if (info.addresses().isEmpty()) {
    fprintf(stderr, "Failed to resolve hostname\n");
    return;
  }
  QString resolvedHost = info.addresses().first().toString();
  this->hostaddr = inet_addr(resolvedHost.toUtf8().constData());
  this->port = htons(atoi(port.toUtf8().constData()));
  this->username = username;
  this->publicKeyPath = publicKeyPath;
  this->privateKeyPath = privateKeyPath;
  this->passPhrase = passPhrase;
  this->authType = 2;
  if (libssh2_init(0) != 0) {
    fprintf(stderr, "libssh2 initialization failed\n");
  }
}

SSHClient::~SSHClient() { stop(); }

bool SSHClient::connect() {
  qDebug() << "开始连接";
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    emit errorMsg("Socket创建失败!");
    return false;
  }
  unsigned long ul = 1;
#ifdef Q_OS_UNIX
  int keepalive = 1;
  //   如该连接在10秒内没有任何数据往来,则进行探测
  int keepidle = 10;
  //  探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
  int keepcount = 3;
  //  每次间隔时间
  int keepintvl = 10;
  setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepcount, sizeof(keepcount));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));

  ioctl(sock, FIONBIO, &ul); //设置为非阻塞模式
#else
  ioctlsocket(sock, FIONBIO, &ul); //设置为非阻塞模式
#endif
  sin.sin_family = AF_INET;
  sin.sin_port = port;
  sin.sin_addr.s_addr = hostaddr;
  int ret =
      ::connect(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
  ret = waitsocket(sock, 30);
  if (ret == 0) {
    emit errorMsg("网络连接超时!");
    return false;
  }
  if (ret < 0) {
    emit errorMsg("网络连接失败!");
    return false;
  }
  qDebug() << "网络连接成功";
  return true;
}

bool SSHClient::openSession() {
  qDebug() << "打开会话";
  LIBSSH2_KNOWNHOSTS *nh;
  /* Open a session */
  session = libssh2_session_init();
  libssh2_session_set_blocking(session, 0);
  int rc = 0;
  while ((rc = libssh2_session_handshake(session, sock)) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    emit errorMsg("Failed to establishing SSH session");
    return false;
  }
  nh = libssh2_knownhost_init(session);
  if (!nh) {
    return false;
  }
  libssh2_knownhost_free(nh);
  return true;
}

bool SSHClient::userauth() {
  qDebug() << "开始认证";
  std::string un = username.toStdString();
  int rc = 0;
  QString errMsg;
  if (authType == 1) {
    std::string p = password.toStdString();
    while ((rc = libssh2_userauth_password(session, un.data(), p.data())) ==
           LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    errMsg = "Authentication by password failed";
  } else if (authType == 2) {
    std::string pkf = publicKeyPath.toStdString();
    std::string pvkf = privateKeyPath.toStdString();
    std::string pp = passPhrase.toStdString();
    while ((rc = libssh2_userauth_publickey_fromfile(
                session, un.data(), pkf.data(), pvkf.data(), pp.data())) ==
           LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    errMsg = "Authentication by public key failed";
  } else {
    fprintf(stderr, "Unknown authentication type: %d\n", authType);
    emit errorMsg("未知的认证类型!");
    return false;
  }

  if (rc) {
    emit errorMsg(errMsg);
    return false;
  }
  emit authSuccess();
  qDebug() << "认证成功";
  return true;
}

bool SSHClient::open_channel() {
  int rc = 0;
  /* Open a channel */
  channel = libssh2_channel_open_session(session);
  while ((channel = libssh2_channel_open_session(session)) == NULL &&
         libssh2_session_last_error(session, NULL, NULL, 0) ==
             LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }

  if (channel == NULL) {
    emit errorMsg("Failed to open a new channel");
    stop();
    return false;
  }

  while ((rc = libssh2_channel_request_pty(channel, "xterm")) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc != 0) {
    emit errorMsg("Failed to request a pty");
    stop();
    return false;
  }
  if (pty_rows != 0 && pty_cols != 0) {
    while ((rc = libssh2_channel_request_pty_size(
                channel, pty_cols, pty_rows)) == LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
  }

  /* Request a shell */
  while ((rc = libssh2_channel_shell(channel)) == LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc != 0) {
    emit errorMsg("Failed to open a shell");
    stop();
    return false;
  }

  emit openChannelSuccess();
  return true;
}

void SSHClient::setChannelRequestPtySize(int row, int column) {
  libssh2_channel_request_pty_size(channel, column, row);
}

void SSHClient::free_channel() {
  if (channel != NULL) {
    libssh2_channel_free(channel);
  }
  channel = NULL;
}

void SSHClient::close_session() {
  if (session != NULL) {
    libssh2_session_disconnect(session,
                               "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);
  }
  session = NULL;
}

void SSHClient::close_connect() {
  if (sock) {
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
  }
}

void SSHClient::exec(QString shell) {
  //  qDebug() << "execCmd ThreadId is" << QThread::currentThreadId();
  QByteArray data = shell.toUtf8();
  int size = data.size();
  //    libssh2_channel_write(channel, shell.toLocal8Bit(), size);
  libssh2_channel_write_ex(channel, 0, data, size);
}

// 获取 UTF-8 字符的字节长度
int utf8_char_length(unsigned char first_byte) {
  if ((first_byte & 0x80) == 0x00)
    return 1; // 0xxxxxxx (ASCII)
  else if ((first_byte & 0xE0) == 0xC0)
    return 2; // 110xxxxx
  else if ((first_byte & 0xF0) == 0xE0)
    return 3; // 1110xxxx (中文常用)
  else if ((first_byte & 0xF8) == 0xF0)
    return 4; // 11110xxx
  return 0;   // 无效格式，按单字节处理
}

void SSHClient::run() {
  qDebug() << "SSHClient ThreadId is" << QThread::currentThreadId();

  bool result = this->connect();
  if (result) {
    result = openSession();
  }
  if (result) {
    result = userauth();
  }

  if (result) {
    result = open_channel();
  }

  if (!result) {
    return;
  }
  emit connectSuccess();
  LIBSSH2_POLLFD fds[2];
  fds[0].type = LIBSSH2_POLLFD_CHANNEL;
  fds[0].fd.channel = channel;
  fds[0].events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_CHANNEL_CLOSED;
  fds[1].type = LIBSSH2_POLLFD_SOCKET;
  fds[1].fd.socket = sock;
  fds[1].events = LIBSSH2_POLLFD_POLLHUP;
  QByteArray buf(READ_BUF_SIZE, 0);
  char leftover[8] = {0}; // 用于保存不完整的多字节字符
  int leftover_len = 0;
  while (running.load()) {
    int act = 0;
    int rc = libssh2_poll(fds, 2, POLL_TIMEOUT);
    if (rc < 1) {
      continue;
    }
    if (libssh2_channel_eof(channel) == 1) {
      emit readChannelData("\n目标主动断开连接");
      emit disconnected();
      break;
    }
    if (fds[0].revents & LIBSSH2_POLLFD_POLLIN) {
      act++;
      ssize_t length = libssh2_channel_read(channel, buf.data() + leftover_len,
                                            READ_BUF_SIZE - leftover_len);
      if (length > 0) {
        // 计算实际数据长度
        int data_len = leftover_len + length;
        // 检查是否有不完整的UTF-8多字节字符
        // UTF-8编码规则：
        // 1字节字符: 0xxxxxxx
        // 2字节字符: 110xxxxx 10xxxxxx
        // 3字节字符: 1110xxxx 10xxxxxx 10xxxxxx
        // 4字节字符: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        int valid_len = data_len;
        while (valid_len > 0) {
          // 检查最后一个字符是否完整
          unsigned char c = buf[valid_len - 1];

          // 如果是ASCII字符(0xxxxxxx)或多字节字符的起始字节(11xxxxxx)，则是完整的
          if ((c & 0x80) == 0 || (c & 0xE0) == 0xC0 || (c & 0xF0) == 0xE0 ||
              (c & 0xF8) == 0xF0) {
            int expectedLength = utf8_char_length(c);
            if (expectedLength > 0 &&
                (valid_len - 1 + expectedLength) <= data_len) {
              valid_len = valid_len - 1 + expectedLength;
              break;
            }
          }
          // 否则是多字节字符的后续字节(10xxxxxx)，可能不完整
          valid_len--;
        }

        // 保存不完整的字符到leftover
        if (valid_len < data_len) {
          // 计算不完整字符的长度
          int partial_len = data_len - valid_len;
          // 复制到leftover
          memcpy(leftover, buf.data() + valid_len, partial_len);
          leftover_len = partial_len;
        }
        // 如果有完整的字符可以处理
        if (valid_len > 0) {
          QByteArray buffer(buf.data(), valid_len);
          QString data = QString::fromUtf8(buffer);
          emit readChannelData(data);
        }

        if (leftover_len > 0) {
          memcpy(buf.data(), leftover, leftover_len);
        }

        if (valid_len == data_len) {
          leftover_len = 0;
        }

      } else if (length == LIBSSH2_ERROR_EAGAIN) {
        continue;
      } else {
        emit readChannelData("\nSSH会话连接异常");
        emit disconnected();
        break;
      }
    }
    if (fds[0].revents & LIBSSH2_POLLFD_CHANNEL_CLOSED ||
        fds[1].revents & LIBSSH2_POLLFD_POLLHUP) {
      /* don't leave loop until we have read all data */
      if (!act) {
        emit readChannelData("\nSSH会话连接关闭");
        emit disconnected();
        break;
      }
    }
  }
  // this->stop();
}

void SSHClient::stop() {
  running.store(false);
  free_channel();
  close_session();
  close_connect();
  quit();
  wait();
}
