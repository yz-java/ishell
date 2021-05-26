#include "welcome.h"
#include "ui_welcome.h"


Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    client=new SSHClient("127.0.0.1","22","root","root");
    client->start();
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::on_send_clicked()
{

}
