#include "sftpclient.h"
#include <QFile>
#include <QEventLoop>
#include <QTimer>
#include <QtConcurrent>
#include <QMutex>

#define BUFFER_SIZE 1048576

QMutex mutexlock;

SFTPClient::SFTPClient(ConnectInfo *connectInfo)
{
    this->connectInfo=*connectInfo;
    rc = libssh2_init(0);
    if(rc != 0) {
        fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
        emit errorMsg("libssh2 initialization failed");
    }
}

bool SFTPClient::connect(){
    qDebug() << "开始连接";
    sock = socket (AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(connectInfo.port);
    sin.sin_addr.s_addr = inet_addr(connectInfo.hostName.toStdString().data());
    if (::connect(sock, (struct sockaddr *) &sin, sizeof(struct sockaddr_in)) != 0) {
        fprintf (stderr, "Failed to established connection!\n");
        emit errorMsg("Failed to established connection!");
        return false;
    }
    emit connectSuccess();
    qDebug() << "连接成功";
    return true;
}

bool SFTPClient::openSession(){
    session = libssh2_session_init();
    if(!session)
        return false;
    libssh2_session_set_blocking(session, 1);
    libssh2_session_handshake(session, sock);
    return true;
}

bool SFTPClient::auth(){
    qDebug() << "开始认证";
    std::string un=connectInfo.username.toStdString();
    int authType=connectInfo.authType;
    if(authType==1){
        std::string p=connectInfo.password.toStdString();
        if (libssh2_userauth_password(session, un.data(), p.data()) != 0) {
               fprintf(stderr, "Failed to authenticate\n");
               emit errorMsg("Failed to authenticate");
               close_connect();
               return false;
         }
    }

    if(authType==2){
        std::string pkf=connectInfo.publicKeyPath.toStdString();
        std::string pvkf=connectInfo.privateKeyPath.toStdString();
        std::string pp=connectInfo.passPhrase.toStdString();
        if (libssh2_userauth_publickey_fromfile(session, un.data(),pkf.data(),pvkf.data(),pp.data()) != 0) {
               fprintf(stderr, "Failed to authenticate\n");
               emit errorMsg("Failed to authenticate");
               close_connect();
               return false;
         }
    }
    emit authSuccess();
    qDebug() << "认证成功";
    return true;
}

bool SFTPClient::initSftpSession(){
    sftp_session = libssh2_sftp_init(session);

    if(!sftp_session) {
        fprintf(stderr, "Unable to init SFTP session\n");
        return false;
    }
    qDebug() << "成功初始化sftp session ";
    emit initSftpSessionSuccess();
    return true;
}

void SFTPClient::opendir(QString sftpPath){
    sftp_handle = libssh2_sftp_opendir(sftp_session, sftpPath.toStdString().data());

        if(!sftp_handle) {
            fprintf(stderr, "Unable to open file with SFTP: %ld\n",
            libssh2_sftp_last_error(sftp_session));
        }
        fprintf(stderr, "libssh2_sftp_open() is done, now receive data!\n");
        do {
            char mem[512];
            char longentry[512];
            LIBSSH2_SFTP_ATTRIBUTES attrs;

            /* loop until we fail */
            rc = libssh2_sftp_readdir_ex(sftp_handle, mem, sizeof(mem),
                                         longentry, sizeof(longentry), &attrs);
            if(rc > 0) {
                /* rc is the length of the file name in the mem
                   buffer */

                if(longentry[0] != '\0') {
                    emit opendirCallBack(longentry);
//                    printf("%s\n", longentry);
                }
            }
            else
                break;

        } while(1);

        libssh2_sftp_close(sftp_handle);
//        libssh2_sftp_shutdown(sftp_session);
}

void SFTPClient::run(){
    this->connect();
    this->openSession();
    this->auth();
    this->initSftpSession();
}

bool SFTPClient::mkdir(QString path){
    rc = libssh2_sftp_mkdir(sftp_session, path.toStdString().data(),
                                LIBSSH2_SFTP_S_IRWXU|
                                LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IXGRP|
                                LIBSSH2_SFTP_S_IROTH|LIBSSH2_SFTP_S_IXOTH);

    if(rc){
        fprintf(stderr, "libssh2_sftp_mkdir failed: %d\n", rc);
        emit errorMsg("创建文件夹失败");
        return false;
    }
    emit successMsg("文件夹创建成功");
    return true;
}

bool SFTPClient::rmdir(QString path){
    std::string p=path.toStdString();
    rc = libssh2_sftp_rmdir_ex(sftp_session,p.c_str(),strlen(p.c_str()));
    if(rc){
        fprintf(stderr, "libssh2_sftp_rmdir failed: %d\n", rc);
        emit errorMsg("删除失败");
        return false;
    }
    return true;
}

void SFTPClient::execShell(QString shell){
    mutexlock.lock();
    string s=shell.toStdString();
    if(channel==NULL){
        channel = libssh2_channel_open_session(session);
        if ( channel == NULL ) {
            fprintf(stderr, "Failed to open a new channel\n");
            emit errorMsg("Failed to open a new channel");
            return;
        }
        if (libssh2_channel_request_pty( channel, "xterm") != 0) {
           fprintf(stderr, "Failed to request a pty\n");
           emit errorMsg("Failed to request a pty");
           return;
        }

        /* Request a shell */
        if (libssh2_channel_shell(channel) != 0) {
           fprintf(stderr, "Failed to open a shell\n");
           emit errorMsg("Failed to open a shell");
           return;
        }
    }
    mutexlock.unlock();
    libssh2_channel_write(channel,s.c_str(),strlen(s.c_str()));
    QTimer::singleShot(1000,[&](){
        free_channel();
    });

}

bool SFTPClient::rmByShell(QString path){
    QString shell="rm -rf "+path+"\r";
    execShell(shell);
    return true;
}

bool SFTPClient::rename(QString sourceName,QString targetName){
    rc = libssh2_sftp_rename(sftp_session, sourceName.toStdString().data(),targetName.toStdString().data());
    if(rc){
        fprintf(stderr, "libssh2_sftp_mkdir failed: %d\n", rc);
        emit errorMsg("重命名失败");
        return false;
    }
    return true;
}

void SFTPClient::fileUpload(QString filePath,QString remotePath){
    sftp_handle =libssh2_sftp_open(sftp_session, remotePath.toStdString().data(),
                          LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
                          LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                          LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
    if(!sftp_handle) {
        fprintf(stderr, "Unable to open file with SFTP: %ld\n",
        libssh2_sftp_last_error(sftp_session));
        return;
    }

    QtConcurrent::run([=](){
        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly))
        {
            emit errorMsg("文件打开失败");
            return;
        }
        int fileSize=f.size();
        int currentSize=0;
        char* data=new char[BUFFER_SIZE];
        int readSize=0;
        while ((readSize=f.read(data,BUFFER_SIZE))>0)
        {
            rc=libssh2_sftp_write(sftp_handle,data,readSize);
            if(rc<=0){
                emit errorMsg("文件上传失败");
                return;
            }
            currentSize+=rc;
            float process=currentSize/(fileSize*1.0);
            emit fileUploadProcess(fileSize,currentSize,process);
        }

        f.close();
        libssh2_sftp_close(sftp_handle);
        emit fileUploadSuccess();
    });

}

void SFTPClient::scpUpload(QString filePath,QString remotePath){
    struct stat fileinfo;
    stat(filePath.toStdString().data(),&fileinfo);
    channel = libssh2_scp_send(session, remotePath.toStdString().data(), fileinfo.st_mode & 0777,
                                  (unsigned long)fileinfo.st_size);
    if(!channel) {
        return;
    }

    QtConcurrent::run([=](){
        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly))
        {
            emit errorMsg("文件打开失败");
            return;
        }
        int fileSize=f.size();
        int currentSize=0;
        char data[BUFFER_SIZE];
        char *ptr;
        int readSize=0;
        while ((readSize=f.read(data,BUFFER_SIZE))>0)
        {
            ptr = data;
            do{
                rc=libssh2_channel_write(channel,ptr,readSize);
                if(rc<0){
                    break;
                }
                currentSize+=rc;
                ptr+=rc;
                readSize-=rc;
                float process=currentSize/(fileSize*1.0);
                emit fileUploadProcess(fileSize,currentSize,process);
            }while (readSize);
        }
        qDebug() << "已发送数据大小："<<currentSize;
        f.close();
        libssh2_channel_send_eof(channel);
        libssh2_channel_wait_eof(channel);
        free_channel();
        emit fileUploadSuccess();
    });

}

void SFTPClient::fileDownload(QString remotePath, QString localPath){
    sftp_handle =libssh2_sftp_open(sftp_session, remotePath.toStdString().data(),
                          LIBSSH2_FXF_READ,
                          LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                          LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
    if(!sftp_handle) {
        fprintf(stderr, "Unable to open file with SFTP: %ld\n",
        libssh2_sftp_last_error(sftp_session));
        return;
    }
    LIBSSH2_SFTP_ATTRIBUTES attributes;
    rc=libssh2_sftp_fstat_ex(sftp_handle,&attributes,0);
    if(rc){
        fprintf(stderr, "libssh2_sftp_mkdir failed: %d\n", rc);
        emit errorMsg("下载失败");
    }
    libssh2_uint64_t fileSize = attributes.filesize;
    qDebug() << "文件大小：" << fileSize;
    QtConcurrent::run([=](){
        QFile file(localPath);
        bool isopen=file.open(QIODevice::WriteOnly);
        if(!isopen){
            emit errorMsg("本地文件打开失败");
            return;
        }
        char* data=new char[BUFFER_SIZE];
        libssh2_uint64_t currentSize=0;
        do{
            rc=libssh2_sftp_read(sftp_handle,data,BUFFER_SIZE);
            if(rc==0){
                break;
            }
            if(rc<0){
                emit errorMsg("文件下载失败");
                return;
            }
            currentSize+=rc;
            file.write(data,rc);
            float process=currentSize/(fileSize*1.0);
            emit fileDownloadProcess(fileSize,currentSize,process);
        }while(rc>0);
        file.close();
        libssh2_sftp_close(sftp_handle);
        emit fileDownloadSuccess();
    });
}

void SFTPClient::scpDownload(QString remotePath, QString localPath){
    libssh2_struct_stat fileinfo;
    channel = libssh2_scp_recv2(session, remotePath.toStdString().data(), &fileinfo);
    if(!channel) {
        fprintf(stderr, "Unable to open channel");
        return;
    }

    libssh2_uint64_t fileSize = fileinfo.st_size;
    qDebug() << "文件大小：" << fileSize;
    QtConcurrent::run([=](){
        QFile file(localPath);
        bool isopen=file.open(QIODevice::WriteOnly);
        if(!isopen){
            emit errorMsg("本地文件打开失败");
            return;
        }
        char* data=new char[BUFFER_SIZE];
        libssh2_uint64_t currentSize=0;
        while (currentSize<fileSize) {
            rc=libssh2_channel_read(channel,data,BUFFER_SIZE);
            if(rc==0){
                break;
            }
            if(rc<0){
                emit errorMsg("文件下载失败");
                return;
            }
            currentSize+=rc;
            file.write(data,rc);
            float process=currentSize/(fileSize*1.0);
            emit fileDownloadProcess(fileSize,currentSize,process);
        }
        file.close();
        free_channel();
        emit fileDownloadSuccess();
    });
}

void SFTPClient::free_channel(){
    if(channel!=NULL){
        libssh2_channel_free(channel);
        channel=NULL;
    }
}

void SFTPClient::close_connect(){
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif

}

void SFTPClient::stop(){
    if(session!=NULL){
        libssh2_session_disconnect(session, "Session Shutdown, Thank you for playing");
        libssh2_session_free(session);
    }
    close_connect();
    libssh2_exit();
}

SFTPClient::~SFTPClient(){

}
