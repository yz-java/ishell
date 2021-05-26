#include "webconsole.h"
#include "ui_webconsole.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "common.h"
#include <QTextCodec>
#include "websocketserver.h"


WebConsole::WebConsole(QWidget *parent,ConnectInfo* connectInfo) :
    QWidget(parent),
    ui(new Ui::WebConsole)
{
    ui->setupUi(this);
    this->connectInfo=*connectInfo;
    qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();

    webChannel = new QWebChannel(this);
    webChannel->registerObject(QStringLiteral("core"), this);

    webView = new QWebEngineView(this);
    webView->page()->setWebChannel(webChannel);

    QFile file(":/html/xterm.html");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QString htmlData = file.readAll().constData();
    webView->setHtml(htmlData);
//    webView->load(QUrl("https://www.baidu.com"));
    webView->show();
    connect(webView,SIGNAL(loadFinished(bool)) ,this,SLOT(pageLoadFinished(bool)));
}

void WebConsole::resizeEvent(QResizeEvent *)
{
    webView->resize(this->size());
}

void WebConsole::pageLoadFinished(bool flag){
    clientId=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
    webView->page()->runJavaScript("setClientId("+clientId+")");
}

void WebConsole::connectSuccess(){


}

void WebConsole::ssh2connect(const QString& jsMsg){
    if(connectInfo.authType==1){
        sshClient=new SSHClient(connectInfo.hostName,QString::number(connectInfo.port),connectInfo.username,connectInfo.password);
    }

    if(connectInfo.authType==2){
        sshClient=new SSHClient(connectInfo.hostName,QString::number(connectInfo.port),connectInfo.username,connectInfo.publicKeyPath,connectInfo.privateKeyPath,connectInfo.passPhrase);
    }
    sshClient->pty_cols=cols;
    sshClient->pty_rows=rows;
    sshClient->start();
    connect(sshClient,SIGNAL(connectSuccess()),this,SLOT(connectSuccess()));

    connect(sshClient,&SSHClient::errorMsg,this,[=](QString errMsg){
        QMessageBox::warning(this,"错误提示",errMsg);
        close();
    });

    connect(sshClient,&SSHClient::authSuccess,this,[=](){

    });

    connect(sshClient,&SSHClient::openChannelSuccess,this,[=](){
        openChannelSeccess=true;
    });


    connect(sshClient,&SSHClient::readChannelData,this,[&](char data){
        //处理中文
        if(data & 0x80){
            ba.append(data);
            if(ba.length()==3){
                QString str(ba);
                ba.clear();
                WebSocketServer::instance->sendMsg(clientId,str);
            }
            return;
        }
        QString d(data);
        WebSocketServer::instance->sendMsg(clientId,d);
    });
}

void WebConsole::recieveJsMessage(const QString& shell){
    sshClient->exec(shell.toStdString().c_str());
//    qDebug() << "input shell：" << shell;
}

void WebConsole::setChannelRequestPtySize(const QString& size){
    QStringList ss=size.split(",");
    rows = ss[0].toInt();
    cols = ss[1].toInt();
    if(openChannelSeccess){
        sshClient->setChannelRequestPtySize(rows,cols);
    }

}

void WebConsole::closeEvent(QCloseEvent *event){
    qDebug() << "close window";
    sshClient->stop();

    QTimer::singleShot(1000,this,[&](){
        WebSocketServer::deleteClient(clientId);
        delete this;
    });
}

WebConsole::~WebConsole()
{
    delete ui;
    delete sshClient;
    delete webView;
    delete webChannel;
}
