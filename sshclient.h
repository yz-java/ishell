#ifndef SSHCLIENT_H
#define SSHCLIENT_H

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libssh2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <QDebug>
#include <QString>
#include <QThread>
#include <iostream>
using namespace std;

#define READ_BUF_SIZE 32000

#ifdef WIN32
#pragma execution_character_set("utf-8")
#else
#include <arpa/inet.h>
#include <libgen.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

class SSHClient : public QThread {
  Q_OBJECT
 public:
  SSHClient(QString hostName, QString port, QString username, QString password);

  SSHClient(QString hostName, QString port, QString username,
            QString publicKeyPath, QString privateKeyPath, QString passPhrase);

  int pty_rows = 0;

  int pty_cols = 0;

  bool connect();

  bool openSession();

  bool userauth();

  bool open_channel();

  void setChannelRequestPtySize(int row, int column);

  void free_channel();

  void close_session();

  void close_connect();

  void stop();

  void exec(QString shell);

 private:
  int sock = 0;

  unsigned long hostaddr = 0;

  int port = 22;

  QString hostName;

  QString username;

  QString password;

  QString publicKeyPath;

  QString privateKeyPath;

  QString passPhrase;

  int authType = 1;

  struct sockaddr_in sin;

  LIBSSH2_SESSION *session = NULL;

  LIBSSH2_CHANNEL *channel = NULL;

  /* For select on stdin */
  fd_set set;

  struct timeval timeval_out;

  void run() override;

 signals:
  void errorMsg(QString errMsg);
  void readChannelData(QString data);
  void connectSuccess();
  void disconnected();
  void authSuccess();
  void openChannelSuccess();
};

#endif  // SSHCLIENT_H
