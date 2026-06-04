#include "sftpclient.h"

#include <QFile>
#include <QHostInfo>
#include <thread>

#define BUFFER_SIZE 1048576

static int waitsocket(libssh2_socket_t socket_fd, LIBSSH2_SESSION *session) {
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

  rc = select((int)(socket_fd + 1), readfd, writefd, NULL, &timeout);

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

SFTPClient::SFTPClient(ConnectInfo connectInfo) : QThread() {
  qRegisterMetaType<FileInfo_S>("FileInfo_S");
  this->connectInfo = connectInfo;
  rc = libssh2_init(0);
  if (rc != 0) {
    fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
    emit errorMsg("libssh2 initialization failed");
  }
  QObject::connect(this, &SFTPClient::asyncScpUpload, this,
                   &SFTPClient::scpUpload);
  QObject::connect(this, &SFTPClient::asyncScpDownload, this,
                   &SFTPClient::scpDownload);
  QObject::connect(this, &SFTPClient::asyncOpendir, this, &SFTPClient::opendir);
  this->moveToThread(this);
}

bool SFTPClient::connect() {
  qDebug() << "开始连接";
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    emit errorMsg("Socket创建失败!");
    return false;
  }
  unsigned long ul = 1;
#ifdef Q_OS_UNIX
  int keepalive = 1;
  //  如该连接在10秒内没有任何数据往来,则进行探测
  int keepidle = 10;
  //  探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
  int keepcount = 3;
  //  每次间隔时间
  int keepintvl = 10;
  setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepcount, sizeof(keepcount));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
  //  设置为非阻塞模式
  ioctl(sock, FIONBIO, &ul);
#else
  //  设置为非阻塞模式
  ioctlsocket(sock, FIONBIO, &ul);
#endif
  sin.sin_family = AF_INET;
  sin.sin_port = htons(connectInfo.port);
  QHostInfo info = QHostInfo::fromName(connectInfo.hostName);
  if (info.addresses().isEmpty()) {
    emit errorMsg("无法解析主机地址!");
    return false;
  }
  QString hostName = info.addresses().first().toString();
  sin.sin_addr.s_addr = inet_addr(hostName.toUtf8().constData());
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

bool SFTPClient::openSession() {
  session = libssh2_session_init();
  if (!session)
    return false;
  libssh2_session_set_blocking(session, 0);
  while ((rc = libssh2_session_handshake(session, sock)) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  return true;
}

bool SFTPClient::auth() {
  qDebug() << "开始认证";
  std::string un = connectInfo.username.toStdString();
  AuthType authType = static_cast<AuthType>(connectInfo.authType);
  if (authType == AuthType::Password) {
    std::string p = connectInfo.password.toStdString();
    while ((rc = libssh2_userauth_password(session, un.data(), p.data())) ==
           LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    if (rc) {
      fprintf(stderr, "Authentication by password failed.\n");
      emit errorMsg("认证失败!");
      close_connect();
      return false;
    }
  } else if (authType == AuthType::PublicKey) {
    std::string pkf = connectInfo.publicKeyPath.toStdString();
    std::string pvkf = connectInfo.privateKeyPath.toStdString();
    std::string pp = connectInfo.passPhrase.toStdString();
    while ((rc = libssh2_userauth_publickey_fromfile(
                session, un.data(), pkf.data(), pvkf.data(), pp.data())) ==
           LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    if (rc) {
      fprintf(stderr, "Authentication by public key failed.\n");
      emit errorMsg("认证失败!");
      close_connect();
      return false;
    }
  } else {
    fprintf(stderr, "Unknown authentication type: %d\n",
            static_cast<int>(authType));
    emit errorMsg("未知的认证类型!");
    close_connect();
    return false;
  }
  emit authSuccess();
  qDebug() << "认证成功";
  return true;
}

bool SFTPClient::initSftpSession() {
  do {
    sftp_session = libssh2_sftp_init(session);

    if (!sftp_session) {
      if (libssh2_session_last_errno(session) == LIBSSH2_ERROR_EAGAIN) {
        fprintf(stderr, "non-blocking init\n");
        waitsocket(sock, session); /* now we wait */
      } else {
        fprintf(stderr, "Unable to init SFTP session\n");
        return false;
      }
    }
  } while (!sftp_session);

  qDebug() << "成功初始化sftp session ";
  emit initSftpSessionSuccess();
  return true;
}

void SFTPClient::opendir(QString sftpPath) {
  emit opendirEvent(sftpPath);
  qDebug() << "opendir ThreadId is" << QThread::currentThreadId();
  LIBSSH2_SFTP_HANDLE *sftp_handle;
  QByteArray pathBytes = sftpPath.toUtf8();
  do {
    sftp_handle = libssh2_sftp_opendir(sftp_session, pathBytes.constData());
    if (!sftp_handle) {
      if (libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN) {
        fprintf(stderr, "Unable to open file with SFTP: %ld\n",
                libssh2_sftp_last_error(sftp_session));

        return;
      } else {
        fprintf(stderr, "non-blocking open\n");
        waitsocket(sock, session); /* now we wait */
      }
    }
  } while (!sftp_handle);

  int i = 0;
  while (1) {
    if (i > 0 && i % 50 == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    i++;
    char fileName[1024] = {0};
    char longentry[1024] = {0};
    LIBSSH2_SFTP_ATTRIBUTES attrs;

    while ((rc = libssh2_sftp_readdir_ex(
                sftp_handle, fileName, sizeof(fileName), longentry,
                sizeof(longentry), &attrs)) == LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    if (rc > 0) {
      QString l(longentry);
      l = QString::number(attrs.mtime) + " " + l;
      if (longentry[0] != '\0') {
        FileInfo_S info = parseBySftpData(l);
        info.filePath = QString("%1/%2").arg(sftpPath).arg(info.fileName);
        if (getFileType(attrs) == 3) {
          bool ret = getFileStat(info.filePath, &attrs);
          if (ret) {
            info.fileType = getFileType(attrs);
          }
        }
        emit opendirCallBack(info);
        emit opendirInfoCallBack(sftpPath, info);
      }
    } else
      break;
  }
  libssh2_sftp_closedir(sftp_handle);
}

void SFTPClient::run() {
  if (!this->connect()) {
    return;
  }
  if (!this->openSession()) {
    return;
  }
  if (!this->auth()) {
    return;
  }
  if (!this->initSftpSession()) {
    return;
  }
  pollThread = std::thread([this]() {
    LIBSSH2_POLLFD fds;
    fds.type = LIBSSH2_POLLFD_SOCKET;
    fds.fd.socket = sock;
    fds.events = LIBSSH2_POLLFD_POLLHUP;
    while (running.load() && libssh2_poll(&fds, 1, 1000) == 0) {
      // 等待连接断开
    }
    if (running.load()) {
      emit disconnected();
    }
  });
  exec();
}

bool SFTPClient::mkdir(QString path) {
  QByteArray pathBytes = path.toUtf8();
  while ((rc = libssh2_sftp_mkdir(
              sftp_session, pathBytes.constData(),
              LIBSSH2_SFTP_S_IRWXU | LIBSSH2_SFTP_S_IRGRP |
                  LIBSSH2_SFTP_S_IXGRP | LIBSSH2_SFTP_S_IROTH |
                  LIBSSH2_SFTP_S_IXOTH)) == LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_mkdir failed: %d\n", rc);
    emit errorMsg("创建文件夹失败");
    return false;
  }
  emit successMsg("文件夹创建成功");
  return true;
}

bool SFTPClient::rmdir(QString path) {
  LIBSSH2_SFTP_HANDLE *sftp_handle;
  QByteArray pathBytes = path.toUtf8();
  do {
    sftp_handle = libssh2_sftp_opendir(sftp_session, pathBytes.constData());
    if (!sftp_handle) {
      if (libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN) {
        fprintf(stderr, "Unable to open file with SFTP: %ld\n",
                libssh2_sftp_last_error(sftp_session));

        return false;
      } else {
        fprintf(stderr, "non-blocking open\n");
        waitsocket(sock, session); /* now we wait */
      }
    }
  } while (!sftp_handle);

  while (1) {
    char fileName[1024] = {0};
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    while ((rc = libssh2_sftp_readdir(sftp_handle, fileName, sizeof(fileName),
                                      &attrs)) == LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    if (rc > 0) {
      QString targetPath =
          QString("%1/%2").arg(path).arg(QString::fromUtf8(fileName));
      if (!strcmp(fileName, ".") || !strcmp(fileName, "..")) {
        continue;
      }
      if (S_ISDIR(attrs.permissions)) {
        rmdir(targetPath);
      } else {
        removeFile(targetPath);
      }
    } else {
      break;
    }
  }
  auto byteArray = path.toUtf8();
  while ((rc = libssh2_sftp_rmdir_ex(sftp_session, byteArray.data(),
                                     byteArray.length())) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_rmdir failed: %d\n", rc);
    emit errorMsg("删除失败");
    libssh2_sftp_closedir(sftp_handle);
    return false;
  }
  libssh2_sftp_closedir(sftp_handle);
  return true;
}

bool SFTPClient::removeFile(QString path) {
  auto byteArray = path.toUtf8();
  while ((rc = libssh2_sftp_unlink_ex(sftp_session, byteArray.data(),
                                      byteArray.length())) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc != 0) {
    return false;
  }
  return true;
}

bool SFTPClient::rename(QString sourceName, QString targetName) {
  while ((rc = libssh2_sftp_rename(
              sftp_session, sourceName.toUtf8().constData(),
              targetName.toUtf8().constData())) == LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_rename failed: %d\n", rc);
    emit errorMsg("重命名失败");
    return false;
  }
  return true;
}

bool SFTPClient::getFileStat(QString filePath, LIBSSH2_SFTP_ATTRIBUTES *attrs) {
  while ((rc = libssh2_sftp_stat(sftp_session, filePath.toUtf8().constData(),
                                 attrs)) == LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_stat failed: %d\n", rc);
    return false;
  }
  return true;
}

int SFTPClient::getFileType(LIBSSH2_SFTP_ATTRIBUTES &attrs) {
  if (LIBSSH2_SFTP_S_ISDIR(attrs.permissions)) {
    return 1;
  } else if (LIBSSH2_SFTP_S_ISREG(attrs.permissions)) {
    return 2;
  } else if (LIBSSH2_SFTP_S_ISLNK(attrs.permissions)) {
    return 3;
  } else if (LIBSSH2_SFTP_S_ISFIFO(attrs.permissions)) {
    return 4;
  } else if (LIBSSH2_SFTP_S_ISSOCK(attrs.permissions)) {
    return 5;
  } else if (LIBSSH2_SFTP_S_ISCHR(attrs.permissions)) {
    return 6;
  } else if (LIBSSH2_SFTP_S_ISBLK(attrs.permissions)) {
    return 7;
  } else {
    return 0;
  }
}

void SFTPClient::scpUpload(QString filePath, QString remotePath) {
  LIBSSH2_SFTP_HANDLE *sftp_handle;
  QByteArray remoteBytes = remotePath.toUtf8();
  do {
    sftp_handle = libssh2_sftp_open(
        sftp_session, remoteBytes.constData(),
        LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,
        LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | LIBSSH2_SFTP_S_IRGRP |
            LIBSSH2_SFTP_S_IROTH);
    if (!sftp_handle &&
        libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN) {
      fprintf(stderr, "Unable to open file with SFTP: %ld\n",
              libssh2_sftp_last_error(sftp_session));

      return;
    }
  } while (!sftp_handle);

  QFile f(filePath);
  if (!f.open(QIODevice::ReadOnly)) {
    emit errorMsg("文件打开失败");
    libssh2_sftp_close(sftp_handle);
    return;
  }
  qint64 fileSize = f.size();
  qint64 currentSize = 0;
  QByteArray data(BUFFER_SIZE, 0);
  char *ptr;
  int readSize = 0;
  while ((readSize = f.read(data.data(), BUFFER_SIZE)) > 0) {
    ptr = data.data();
    do {
      while ((rc = libssh2_sftp_write(sftp_handle, ptr, readSize)) ==
             LIBSSH2_ERROR_EAGAIN) {
        waitsocket(sock, session);
      }
      if (rc < 0)
        break;
      currentSize += rc;
      ptr += rc;
      readSize -= rc;
      float process = currentSize / (fileSize * 1.0);
      emit fileUploadProcess(fileSize, currentSize, process);
    } while (readSize);
  }
  qDebug() << "已发送数据大小：" << currentSize;
  f.close();
  libssh2_sftp_close(sftp_handle);
  emit fileUploadSuccess();
}

void SFTPClient::scpDownload(QString remotePath, QString localPath) {
  LIBSSH2_SFTP_ATTRIBUTES attrs;
  LIBSSH2_SFTP_HANDLE *sftp_handle;
  QByteArray remoteBytes = remotePath.toUtf8();
  do {
    sftp_handle = libssh2_sftp_open(sftp_session, remoteBytes.constData(),
                                    LIBSSH2_FXF_READ, 0);
    if (!sftp_handle) {
      if (libssh2_session_last_errno(session) != LIBSSH2_ERROR_EAGAIN) {
        fprintf(stderr, "Unable to open file with SFTP: %ld\n",
                libssh2_sftp_last_error(sftp_session));

        return;
      } else {
        fprintf(stderr, "non-blocking open\n");
        waitsocket(sock, session); /* now we wait */
      }
    }
  } while (!sftp_handle);
  while ((rc = libssh2_sftp_fstat(sftp_handle, &attrs)) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_fstat failed.\n");
    libssh2_sftp_close(sftp_handle);
    return;
  }

  QFile file(localPath);
  bool isopen = file.open(QIODevice::WriteOnly);
  if (!isopen) {
    emit errorMsg("本地文件打开失败");
    libssh2_sftp_close(sftp_handle);
    return;
  }
  QByteArray data(BUFFER_SIZE, 0);
  libssh2_uint64_t currentSize = 0;
  libssh2_uint64_t fileSize = attrs.filesize;
  do {
    /* read in a loop until we block */
    while ((rc = libssh2_sftp_read(sftp_handle, data.data(), BUFFER_SIZE)) ==
           LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    if (rc < 0) {
      emit errorMsg("文件下载失败");
      file.close();
      libssh2_sftp_close(sftp_handle);
      return;
    }
    if (rc == 0) {
      break;
    }
    currentSize += rc;
    file.write(data.data(), rc);
    float process = currentSize / (fileSize * 1.0);
    emit fileDownloadProcess(fileSize, currentSize, process);
  } while (rc > 0);

  file.close();
  libssh2_sftp_close(sftp_handle);
  emit fileDownloadSuccess();
}

void SFTPClient::close_connect() {
#ifdef WIN32
  closesocket(sock);
#else
  close(sock);
#endif
}

void SFTPClient::stop() {

  if (sftp_session) {
    libssh2_sftp_shutdown(sftp_session);
    sftp_session = NULL;
  }

  if (session) {
    libssh2_session_disconnect(session,
                               "Session Shutdown, Thank you for playing");
    libssh2_session_free(session);
    session = NULL;
  }
  close_connect();
  running.store(false);
  if (pollThread.joinable()) {
    pollThread.join();
  }
  quit();
  wait();
}

SFTPClient::~SFTPClient() { stop(); }
