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
        QString script;
        //处理中文
        if(data & 0x80){
            ba.append(data);
            if(ba.length()==3){
                QString str(ba);
                ba.clear();
                script="term.write('"+str+"')";
                webView->page()->runJavaScript(script);
            }
            return;
        }
        if(data==13){
           script="lineFeed()";
        }else{
            QString d(data);
            script="term.write('"+d+"')";
        }

        webView->page()->runJavaScript(script);
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
