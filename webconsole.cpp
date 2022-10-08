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

    QFile file(":/html/xterm4/index.html");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QString htmlData = file.readAll().constData();
    webView->setHtml(htmlData);
//    webView->load(QUrl("http://localhost:8080"));
    webView->show();
    connect(webView,SIGNAL(loadFinished(bool)) ,this,SLOT(pageLoadFinished(bool)));
}

void WebConsole::resizeEvent(QResizeEvent *)
{
    webView->resize(this->size());
}

void WebConsole::pageLoadFinished(bool flag){
//    clientId=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
//    webView->page()->runJavaScript("setClientId("+clientId+")");
    this->ssh2connect("");
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
        QTimer::singleShot(1000,this,[=]{
            webView->page()->runJavaScript("connectSuccess()");
        });
    });


    connect(sshClient,&SSHClient::readChannelData,this,[&](QString data){
        QJsonObject obj;
        QJsonDocument doc;
        obj["data"]=data;
        doc.setObject(obj);
        data=doc.toJson(QJsonDocument::Compact);
        webView->page()->runJavaScript("xtermWrite("+data+")");
//        WebSocketServer::instance->sendMsg(clientId,data);
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

void WebConsole::paintEvent(QPaintEvent *event)
{
//    Q_UNUSED(event);
//    QPainter p(this);
//    p.setPen(Qt::NoPen);
//    p.setBrush(Qt::red);
//    p.drawRect(rect());
}

WebConsole::~WebConsole()
{
    ba.clear();
    delete ui;
    delete sshClient;
    delete webView;
    delete webChannel;
}
