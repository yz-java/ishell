#include "console.h"
#include "ui_console.h"
#include <QPainter>
#include <QVBoxLayout>

Console::Console(QWidget *parent) :
    QAbstractScrollArea(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sshClient=new SSHClient("127.0.0.1","22","developer","yangzhao!3329");
    sshClient->start();
    connect(sshClient,SIGNAL(connectSuccess()),this,SLOT(connectSuccess()));
    connect(sshClient,&SSHClient::authSuccess,[=](){
        qDebug() << "认证成功";
    });

    connect(sshClient,SIGNAL(readChannelData(const char)),this,SLOT(readChannelData(const char)));
    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void Console::resizeEvent(QResizeEvent *){

}

void Console::paintEvent(QPaintEvent *event){
//    QPainter painter(this->viewport());
//    // 设置画笔颜色
//    painter.setPen(QColor(0, 160, 230));
//    painter.drawText(100, 100, QStringLiteral("青春不老，奋斗不止！"));

//    // 绘制区域从坐标点(20, 200)处开始
//    painter.drawText(QPoint(20, 200), QStringLiteral("纯正开源之美，有趣、好玩、靠谱。。。"));

}

void Console::keyPressEvent(QKeyEvent *event){


}

void Console::mousePressEvent(QMouseEvent *event){


}

void Console::connectSuccess(){

}

void Console::readChannelData(const char data){
    QString d(data);
    lock.lock();
    if(data=='\r'){
        this->lineEdit=NULL;
    }
    if(this->lineEdit==NULL){
        this->lineEdit=new QLineEdit;
        this->lineEdit->setEnabled(false);
        this->layout->addWidget(lineEdit);
    }
    QString o =this->lineEdit->text();
    this->lineEdit->setText(o+d);
    lock.unlock();
}

Console::~Console()
{
    delete ui;
}
