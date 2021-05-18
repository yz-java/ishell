﻿#include "sshclient.h"
#include <QTimer>

SSHClient::SSHClient(QObject *parent)
{

}
SSHClient::SSHClient(QString hostName,QString port,QString username,const QString password){
    this->hostaddr = inet_addr(hostName.toStdString().data());
    this->port =  htons(atoi(port.toStdString().data()));
    this->username = username;
    this->password = password;
    if (libssh2_init (0) != 0) {
        fprintf (stderr, "libssh2 initialization failed\n");
    }
}

SSHClient::SSHClient(QString hostName,QString port,QString username,QString publicKeyPath,QString privateKeyPath,QString passPhrase){
    this->hostaddr = inet_addr(hostName.toStdString().data());
    this->port =  htons(atoi(port.toStdString().data()));
    this->username = username;
    this->publicKeyPath = publicKeyPath;
    this->privateKeyPath=privateKeyPath;
    this->passPhrase=passPhrase;
    this->authType=2;
    if (libssh2_init (0) != 0) {
        fprintf (stderr, "libssh2 initialization failed\n");
    }
}

bool SSHClient::connect(){
    qDebug() << "开始连接";
    sock = socket (AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = port;
    sin.sin_addr.s_addr = hostaddr;
    if (::connect(sock, (struct sockaddr *) &sin, sizeof(struct sockaddr_in)) != 0) {
        fprintf (stderr, "Failed to established connection!\n");
        return false;
    }
    emit connectSuccess();
    qDebug() << "连接成功";
    return true;
}

bool SSHClient::openSession(){
    qDebug() << "打开会话";
    /* Open a session */
    session = libssh2_session_init();
    if (libssh2_session_startup (session, sock) != 0) {
        fprintf(stderr, "Failed Start the SSH session\n");
        return false;
    }
    qDebug() << "打开会话成功";
    return true;
}

bool SSHClient::userauth(){
    qDebug() << "开始认证";
    std::string un=username.toStdString();
    if(authType==1){
        std::string p=password.toStdString();
        if (libssh2_userauth_password(session, un.data(), p.data()) != 0) {
               fprintf(stderr, "Failed to authenticate\n");
               close(sock);
               close_connect();
               return false;
         }
    }

    if(authType==2){
        std::string pkf=publicKeyPath.toStdString();
        std::string pvkf=privateKeyPath.toStdString();
        std::string pp=passPhrase.toStdString();
        if (libssh2_userauth_publickey_fromfile(session, un.data(),pkf.data(),pvkf.data(),pp.data()) != 0) {
               fprintf(stderr, "Failed to authenticate\n");
               close(sock);
               close_connect();
               return false;
         }
    }
    emit authSuccess();
    qDebug() << "认证成功";
    return true;
}

bool SSHClient::open_channel(){
    /* Open a channel */
    channel = libssh2_channel_open_session(session);

    if ( channel == NULL ) {
        fprintf(stderr, "Failed to open a new channel\n");
        stop();
        return false;
    }

    if (libssh2_channel_request_pty( channel, "xterm") != 0) {
       fprintf(stderr, "Failed to request a pty\n");
       stop();
       return false;
    }
    if(pty_rows!=0&&pty_cols!=0){
        libssh2_channel_request_pty_size(channel, pty_cols, pty_rows);
    }

    /* Request a shell */
    if (libssh2_channel_shell(channel) != 0) {
       fprintf(stderr, "Failed to open a shell\n");
       stop();
       return false;
    }

    emit openChannelSuccess();
    return true;
}

void SSHClient::setChannelRequestPtySize(int row,int column){
    libssh2_channel_request_pty_size(channel, column, row);
}
void SSHClient::free_channel(){
    libssh2_channel_free(channel);
    channel=NULL;
}

void SSHClient::close_session(){
    libssh2_session_disconnect(session, "Session Shutdown, Thank you for playing");
    libssh2_session_free(session);
}

void SSHClient::close_connect(){
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

void SSHClient::exec(std::string shell){
    int size=shell.size();
    libssh2_channel_write_ex(channel, 0, shell.c_str(), size);
//    libssh2_channel_write(channel,shell.c_str(),1);
}

void SSHClient::run(){
    bool result=this->connect();
    if(result){
        result=openSession();
    }
    if(result){
        result=userauth();
    }

    if(result){
        result=open_channel();
    }

    if(!result){
        return;
    }

    int nfds = 1;
    char buf;
    while (running) {
        if ((fds = static_cast<LIBSSH2_POLLFD *>(malloc(sizeof(LIBSSH2_POLLFD)))) == NULL)
            break;
        fds[0].type = LIBSSH2_POLLFD_CHANNEL;
        fds[0].fd.channel = channel;
        fds[0].events =  LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

        if (libssh2_poll(fds, nfds, 10) >0) {
            libssh2_channel_read(channel, &buf, 1);
            emit readChannelData(buf);
//            fprintf(stdout, "%c", buf);
//            fflush(stdout);
        }
        free (fds);
        if (libssh2_channel_eof(channel) == 1)
            break;
    }
}

void SSHClient::stop(){
    running=false;
    QTimer::singleShot(100,this,[&](){
        close_session();
        close_connect();
        libssh2_exit();
    });


}
