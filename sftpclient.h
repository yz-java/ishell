#ifndef SFTPCLIENT_H
#define SFTPCLIENT_H

#include <QObject>
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
#include <libssh2_sftp.h>
#include <QEventLoop>
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

#include "connectinfo.h"

class SFTPClient : public QThread
{
    Q_OBJECT
public:
    explicit SFTPClient(ConnectInfo *connectInfo = nullptr);
    ~SFTPClient();
    bool connect();

    bool openSession();

    bool auth();

    bool initSftpSession();

    void opendir(QString sftpPath);

    bool mkdir(QString path);

    bool rmdir(QString path);

    bool rmByShell(QString path);

    bool rename(QString sourceName,QString targetName);

    void fileUpload(QString filePath,QString remotePath);

    void fileDownload(QString remotePath,QString localPath);

    void execShell(QString shell);

    void run();

    void stop();

private:
    ConnectInfo connectInfo;
    QEventLoop loop;
    int sock = 0;
    struct sockaddr_in sin;
    int rc=0;
    LIBSSH2_SESSION *session=NULL;
    LIBSSH2_CHANNEL *channel=NULL;
    LIBSSH2_SFTP *sftp_session=NULL;
    LIBSSH2_SFTP_HANDLE *sftp_handle=NULL;

    void close_connect();


signals:
    void errorMsg(QString msg);
    void successMsg(QString msg);
    void connectSuccess();
    void authSuccess();
    void opendirCallBack(QString data);
    void initSftpSessionSuccess();

    void fileUploadProcess(int fileSize,int currentSize,float process);
    void fileUploadSuccess();

    void fileDownloadProcess(int fileSize,int currentSize,float process);
    void fileDownloadSuccess();

};

#endif // SFTPCLIENT_H
