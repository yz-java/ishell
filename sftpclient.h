#ifndef SFTPCLIENT_H
#define SFTPCLIENT_H

#include "fileinfo.h"
#include <QDebug>
#include <QEventLoop>
#include <QObject>
#include <QString>
#include <QThread>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

using namespace std;

#define BUFSIZE 32000

#ifdef WIN32
#pragma execution_character_set("utf-8")
#else
#include <arpa/inet.h>
#include <libgen.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include "connectinfo.h"
class SFTPClient : public QThread {
  Q_OBJECT
public:
  explicit SFTPClient(ConnectInfo connectInfo);
  ~SFTPClient();
  bool connect();

  bool openSession();

  bool auth();

  bool initSftpSession();

  bool mkdir(QString path);

  bool rmdir(QString path);

  bool removeFile(QString path);

  bool rename(QString sourceName, QString targetName);

  bool getFileStat(QString filePath, LIBSSH2_SFTP_ATTRIBUTES *attrs);

  int getFileType(LIBSSH2_SFTP_ATTRIBUTES &attrs);

  void run();

  void stop();

private:
  ConnectInfo connectInfo;

  int sock = 0;

  struct sockaddr_in sin;
  int rc = 0;
  LIBSSH2_SESSION *session = NULL;
  LIBSSH2_SFTP *sftp_session = NULL;

  void close_connect();

public slots:
  void scpUpload(QString filePath, QString remotePath);

  void scpDownload(QString filePath, QString remotePath);

  void opendir(QString sftpPath);

signals:
  void asyncScpUpload(QString filePath, QString remotePath);

  void asyncScpDownload(QString filePath, QString remotePath);

  void asyncOpendir(QString sftpPath);

  void errorMsg(QString msg);
  void successMsg(QString msg);
  void connectSuccess();
  void authSuccess();

  void opendirCallBack(FileInfo_S info);

  void opendirInfoCallBack(QString dirPath, FileInfo_S info);

  void initSftpSessionSuccess();

  void fileUploadProcess(int fileSize, int currentSize, float process);
  void fileUploadSuccess();

  void fileDownloadProcess(int fileSize, int currentSize, float process);
  void fileDownloadSuccess();

  void opendirEvent(QString dirPath);
  void disconnected();
};

#endif // SFTPCLIENT_H
