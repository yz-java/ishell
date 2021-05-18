#ifndef SSHCLIENT_H
#define SSHCLIENT_H

#include <QThread>
#include <QString>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdlib.h>
#include <libssh2.h>
#include <QDebug>
#include <iostream>
using namespace std;

#define BUFSIZE 32000

#ifdef WIN32
#pragma execution_character_set("utf-8")
#else
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <libgen.h>
#endif

class SSHClient : public QThread
{
    Q_OBJECT
public:
    explicit SSHClient(QObject *parent = nullptr);

    SSHClient(QString hostName,QString port,QString username,QString password);

    SSHClient(QString hostName,QString port,QString username,QString publicKeyPath,QString privateKeyPath,QString passPhrase);

    int pty_rows=0;

    int pty_cols=0;

    bool connect();

    bool openSession();

    bool userauth();

    bool open_channel();

    void setChannelRequestPtySize(int row,int column);

    void exec(std::string shell);

    void free_channel();

    void close_session();

    void close_connect();

    void stop();

private:
    int sock = 0;

    unsigned long hostaddr = 0;

    int port = 22;

    QString username;

    QString password;

    QString publicKeyPath;

    QString privateKeyPath;

    QString passPhrase;

    int authType=1;

    struct sockaddr_in sin;

    LIBSSH2_SESSION *session=NULL;

    LIBSSH2_CHANNEL *channel=NULL;

    LIBSSH2_POLLFD *fds = NULL;

    /* For select on stdin */
    fd_set set;

    struct timeval timeval_out;

    bool running=true;

    void run();


signals:
    void errorMsg(QString errMsg);
    void readChannelData(char data);
    void connectSuccess();
    void authSuccess();
    void openChannelSuccess();
};

#endif // SSHCLIENT_H
