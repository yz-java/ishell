#include "connecteditui.h"
#include "ui_connecteditui.h"
#include <QFileDialog>
#include "db/connectdao.h"
#include <QMessageBox>

ConnectEditUI::ConnectEditUI(QWidget *parent,QString title) :
    QDialog(parent),
    ui(new Ui::ConnectEditUI)
{
    ui->setupUi(this);
    setWindowTitle(title);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());
    ui->save->setIcon(QIcon(":/icons/save.png"));
    ui->cancel->setIcon(QIcon(":/icons/cancel.png"));
    refreshPage();
}

void ConnectEditUI::refreshPage(){
    ui->name->setText(info.name);
    ui->hostName->setText(info.hostName);
    ui->port->setText(QString::number(info.port));
    ui->authType->setCurrentIndex(info.authType-1);
    ui->userName->setText(info.username);
    ui->password->setText(info.password);
    ui->publickKeyFilePath->setText(info.publicKeyPath);
    ui->privateKeyFilePath->setText(info.privateKeyPath);
    ui->passPhrase->setText(info.passPhrase);
    int authType=0;
    if(info.authType-1>0){
        authType=info.authType-1;
    }
    ui->authType->setCurrentIndex(authType);
//    this->on_authType_currentIndexChanged(authType);
}

ConnectEditUI::~ConnectEditUI()
{
    delete ui;
}

void ConnectEditUI::on_authType_currentIndexChanged(int index)
{
    if(index==0){
        ui->privateKeyFilePath->clear();
        ui->privateKeyFilePath->setEnabled(false);
        ui->publickKeyFilePath->clear();
        ui->publickKeyFilePath->setEnabled(false);
        ui->passPhrase->clear();
        ui->passPhrase->setEnabled(false);
        ui->selectPrivateKeyFileButton->setEnabled(false);
        ui->selectPublicKeyFileButton->setEnabled(false);

        ui->password->setEnabled(true);
    }
    if(index==1){
        ui->password->clear();
        ui->password->setEnabled(false);

        ui->publickKeyFilePath->setEnabled(true);
        ui->privateKeyFilePath->setEnabled(true);
        ui->passPhrase->setEnabled(true);
        ui->selectPublicKeyFileButton->setEnabled(true);
        ui->selectPrivateKeyFileButton->setEnabled(true);
    }
}

void ConnectEditUI::on_selectPrivateKeyFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("open file"), "",  tr("*"));
    ui->privateKeyFilePath->setText(filePath);
}

void ConnectEditUI::on_save_clicked()
{
    info.name=ui->name->text();
    info.hostName=ui->hostName->text();
    info.port=ui->port->text().toInt();
    info.authType=ui->authType->currentIndex()+1;
    info.username=ui->userName->text();
    info.password=ui->password->text();
    info.publicKeyPath=ui->publickKeyFilePath->text();
    info.privateKeyPath=ui->privateKeyFilePath->text();
    info.passPhrase=ui->passPhrase->text();
    if(info.parentId==0){
        info.parentId=info.id;
        bool result=ConnectDao::GetInstance()->addConnectInfo(&info);
        if(result){
            emit addSuccess(info);
            QMessageBox::information(this,"提示","资源添加成功");
        }
    }else{
        bool result=ConnectDao::GetInstance()->updateConnectById(info);
        if(result){
            emit updateSuccess(info);
            QMessageBox::information(this,"提示","资源更新成功");
            this->close();
        }
    }
}

void ConnectEditUI::setConnectInfo(ConnectInfo info){
    this->info=info;
    refreshPage();
}

void ConnectEditUI::on_selectPublicKeyFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("open file"), "",  tr("*"));
    ui->publickKeyFilePath->setText(filePath);
}

void ConnectEditUI::on_cancel_clicked()
{
    this->close();
}
