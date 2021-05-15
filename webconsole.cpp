#include "webconsole.h"
#include "ui_webconsole.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "common.h"
#include <QTextCodec>


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

    if(connectInfo.authType==1){
        sshClient=new SSHClient(connectInfo.hostName,QString::number(connectInfo.port),connectInfo.username,connectInfo.password);
    }

    if(connectInfo.authType==2){
        sshClient=new SSHClient(connectInfo.hostName,QString::number(connectInfo.port),connectInfo.username,connectInfo.publicKeyPath,connectInfo.privateKeyPath,connectInfo.passPhrase);
    }


    sshClient->start();
    connect(sshClient,SIGNAL(connectSuccess()),this,SLOT(connectSuccess()));
    connect(sshClient,&SSHClient::authSuccess,[=](){

    });
    connect(sshClient,&SSHClient::readChannelData,this,[&](char data){
        QString script;
        if(data==13){
           script="lineFeed()";
        }else{
            QString d(data);
            script="term.write('"+d+"')";
        }

        webView->page()->runJavaScript(script);
    });
}

void WebConsole::connectSuccess(){


}

void WebConsole::recieveJsMessage(const QString& shell){
    sshClient->exec(shell.toStdString().c_str());
    qDebug() << "input shell：" << shell;
}


WebConsole::~WebConsole()
{
    delete ui;
    delete sshClient;
    delete webView;
    delete webChannel;
}
