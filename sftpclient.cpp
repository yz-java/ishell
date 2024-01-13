#include "sftpclient.h"

#include <QEventLoop>
#include <QFile>
#include <QHostInfo>
#include <QMutex>
#include <QTimer>
#include <QtConcurrent>

#include "fileinfo.h"
#define BUFFER_SIZE 1048576

QMutex mutexlock;

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

SFTPClient::SFTPClient(ConnectInfo connectInfo) : QThread() {
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
#ifdef UNIX
  int keepalive = 1;
  int keepidle = 10; // 如该连接在10秒内没有任何数据往来,则进行探测
  int keepcount =
      3; //探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
  int keepintvl = 10; //每次间隔时间
  setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepcount, sizeof(keepcount));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
#endif
  sin.sin_family = AF_INET;
  sin.sin_port = htons(connectInfo.port);
  QHostInfo info = QHostInfo::fromName(connectInfo.hostName);
  QString hostName = info.addresses().first().toString();
  sin.sin_addr.s_addr = inet_addr(hostName.toStdString().data());
  if (::connect(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) !=
      0) {
    fprintf(stderr, "Failed to established connection!\n");
    emit errorMsg("网络异常!");
    return false;
  }
  emit connectSuccess();
  qDebug() << "连接成功";
  return true;
}

bool SFTPClient::openSession() {
  session = libssh2_session_init();
  if (!session)
    return false;
  libssh2_session_set_blocking(session, 0);
  // libssh2_session_handshake(session, sock);
  while ((rc = libssh2_session_handshake(session, sock)) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  // libssh2_session_set_timeout(session, 0);
  return true;
}

bool SFTPClient::auth() {
  qDebug() << "开始认证";
  std::string un = connectInfo.username.toStdString();
  int authType = connectInfo.authType;
  if (authType == 1) {
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
  }

  if (authType == 2) {
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
  do {
    sftp_handle =
        libssh2_sftp_opendir(sftp_session, sftpPath.toStdString().data());
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
        emit opendirCallBack(l);
        emit opendirInfoCallBack(sftpPath, l);
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
  std::thread t([=]() {
    LIBSSH2_POLLFD *fds = NULL;
    if ((fds = static_cast<LIBSSH2_POLLFD *>(malloc(sizeof(LIBSSH2_POLLFD)))) ==
        NULL) {
      return;
    }
    fds[0].type = LIBSSH2_POLLFD_SOCKET;
    fds[0].fd.socket = sock;
    fds[0].events = LIBSSH2_POLLFD_POLLHUP;
    while (true) {
        int rc =libssh2_poll(fds, 1, 1000);
        if(rc>0){
            break;
        }
        continue;
    }
    if (fds) {
      free(fds);
      fds = NULL;
    }
    emit disconnected();
  });
  t.detach();
  exec();
}

bool SFTPClient::mkdir(QString path) {
  while ((rc = libssh2_sftp_mkdir(
              sftp_session, path.toStdString().data(),
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
  std::string p = path.toStdString();
  while ((rc = libssh2_sftp_rmdir_ex(sftp_session, p.c_str(),
                                     strlen(p.c_str()))) ==
         LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_rmdir failed: %d\n", rc);
    emit errorMsg("删除失败");
    return false;
  }
  return true;
}

bool SFTPClient::removeFile(QString path) {
  string filePath = path.toStdString();
  while ((rc = libssh2_sftp_unlink_ex(sftp_session, filePath.data(),
                                      filePath.length())) ==
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
              sftp_session, sourceName.toStdString().data(),
              targetName.toStdString().data())) == LIBSSH2_ERROR_EAGAIN) {
    waitsocket(sock, session);
  }
  if (rc) {
    fprintf(stderr, "libssh2_sftp_mkdir failed: %d\n", rc);
    emit errorMsg("重命名失败");
    return false;
  }
  return true;
}

void SFTPClient::scpUpload(QString filePath, QString remotePath) {
  struct stat fileinfo;
  stat(filePath.toStdString().data(), &fileinfo);
  LIBSSH2_SFTP_HANDLE *sftp_handle;
  do {
    sftp_handle = libssh2_sftp_open(
        sftp_session, remotePath.toStdString().c_str(),
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
    return;
  }
  int fileSize = f.size();
  int currentSize = 0;
  char *data = new char[BUFFER_SIZE];
  char *ptr;
  int readSize = 0;
  while ((readSize = f.read(data, BUFFER_SIZE)) > 0) {
    ptr = data;
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
  delete[] data;
  qDebug() << "已发送数据大小：" << currentSize;
  f.close();
  emit fileUploadSuccess();
}

void SFTPClient::scpDownload(QString remotePath, QString localPath) {
  LIBSSH2_SFTP_ATTRIBUTES attrs;
  LIBSSH2_SFTP_HANDLE *sftp_handle;
  do {
    sftp_handle = libssh2_sftp_open(
        sftp_session, remotePath.toStdString().c_str(), LIBSSH2_FXF_READ, 0);
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
    return;
  }

  QFile file(localPath);
  bool isopen = file.open(QIODevice::WriteOnly);
  if (!isopen) {
    emit errorMsg("本地文件打开失败");
    return;
  }
  char *data = new char[BUFFER_SIZE];
  libssh2_uint64_t currentSize = 0;
  libssh2_uint64_t fileSize = attrs.filesize;
  do {
    /* read in a loop until we block */
    while ((rc = libssh2_sftp_read(sftp_handle, data, BUFFER_SIZE)) ==
           LIBSSH2_ERROR_EAGAIN) {
      waitsocket(sock, session);
    }
    if (rc < 0) {
      emit errorMsg("文件下载失败");
      return;
    }
    if (rc == 0) {
      break;
    }
    currentSize += rc;
    file.write(data, rc);
    float process = currentSize / (fileSize * 1.0);
    emit fileDownloadProcess(fileSize, currentSize, process);
  } while (rc > 0);

  delete[] data;
  file.close();
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
  libssh2_exit();
  quit();
  wait();
}

SFTPClient::~SFTPClient() {
  if (sftp_session) {
    libssh2_sftp_shutdown(sftp_session);
  }

  if (session) {
    libssh2_session_disconnect(session,
                               "Session Shutdown, Thank you for playing");
    libssh2_session_free(session);
  }
  running = 0;
  if (fds) {
    free(fds);
    fds = NULL;
  }
}
