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

    bool connect();

    bool openSession();

    bool userauth();

    bool open_channel();

    void setChannelRequestPtySize(int row,int column);

    void exec(std::string shell);

    void close_channel();

    void close_connect();



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
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    int nfds = 1;
    LIBSSH2_POLLFD *fds = NULL;

    /* Struct winsize for term size */
//    struct winsize w_size;
//    struct winsize w_size_bck;

    /* For select on stdin */
    fd_set set;
    struct timeval timeval_out;

    char buf;
    void run();


signals:
//    void readChannelData(const char* data);
    void readChannelData(char data);
    void connectSuccess();
    void authSuccess();
};

#endif // SSHCLIENT_H
