#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    sshClient=new SSHClient("127.0.0.1","22","root","root");
    sshClient->start();
    connect(sshClient,SIGNAL(connectSuccess()),this,SLOT(connectSuccess()));
    connect(sshClient,&SSHClient::authSuccess,[=](){
        qDebug() << "认证成功";
    });

    connect(sshClient,SIGNAL(readChannelData(const char*)),this,SLOT(readChannelData(const char*)));
}

void Console::resizeEvent(QResizeEvent *){

}

void Console::connectSuccess(){

}

void Console::readChannelData(const char* data){
    QString d(data);
//    qDebug() << data;
//        QString script="writeData('"+d+"')";
        ui->console->appendHtml(d);
}

Console::~Console()
{
    delete ui;
}
