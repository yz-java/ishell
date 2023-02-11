#include "sftpdialog.h"
#include "ui_sftpdialog.h"
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include "fileinfo.h"
#include <QMenu>
#include "components/confirmdialog.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QtConcurrent>

SftpDialog::SftpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SftpDialog)
{
    ui->setupUi(this);
}

SftpDialog::SftpDialog(QWidget *parent,ConnectInfo* connectInfo) :
    QDialog(parent),
    ui(new Ui::SftpDialog)
{
    ui->setupUi(this);
    this->connectInfo=connectInfo;
    setWindowTitle(connectInfo->name+"-文件管理");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    sftpClient=new SFTPClient(connectInfo);
    sftpClient->start();
    initUI();
    connect(sftpClient,&SFTPClient::initSftpSessionSuccess,this,[=](){
        sftpClient->opendir(rootDir);
    });
    connect(sftpClient,&SFTPClient::errorMsg,this,[=](QString msg){
        QMessageBox::warning(this,"错误提示",msg);
    });
    connect(sftpClient,&SFTPClient::successMsg,this,[=](QString msg){
        QMessageBox::information(this,"提示",msg);
    });
    connect(sftpClient,&SFTPClient::opendirEvent,this,[=](QString currentPath){
        if(currentPath=="/"){
            refresh=true;
            int count = rootItem->childCount();
            for(int i = 0;i<count;i++){
                rootItem->removeChild(rootItem->takeChild(0));
            }
        }else{
            refresh=false;
        }
    });
    connect(sftpClient,&SFTPClient::opendirInfoCallBack,this,[=](QString currentPath,QString data){
        if(!refresh){
            return;
        }
        FileInfo_S info = parseBySftpData(data);
        if(info.fileName=="."||info.fileName==".."){
            return;
        }
        QTreeWidgetItem* item = new QTreeWidgetItem(rootItem);
        item->setText(0,info.fileName);
        item->setText(1,info.fileSize);
        item->setText(2,info.fileNum);

        if(info.fileType==1){
            item->setIcon(0,QIcon(":/icons/folder.png"));
            item->setText(3,"目录");
        }else if(info.fileType==2){
            item->setIcon(0,QIcon(":/icons/file.png"));
            item->setText(3,"文件");
        }else if(info.fileType==3){
            item->setIcon(0,QIcon(":/icons/folder.png"));
            item->setText(3,"符号链接");

        }
        item->setText(4,info.permission);
        item->setText(5,info.userOrGroup);

        item->setText(6,currentPath+"/"+info.fileName);
        info.filePath=currentPath+"/"+info.fileName;
        item->setData(0,Qt::UserRole+1,QVariant::fromValue(info));

        item->setText(7,info.updateTime);
    });
}

void SftpDialog::initUI(){
    QHBoxLayout *layout=new QHBoxLayout;
    treeView=new QTreeWidget(this);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->sortByColumn(0,Qt::SortOrder::AscendingOrder);
    treeView->setItemsExpandable(true);
    QFont font;
    font.setPointSize(12);
    treeView->setFont(font);
    rootItem = new QTreeWidgetItem(treeView);
    rootItem->setText(0,"/");
    rootItem->setText(6,"/");
    rootItem->setIcon(0,QIcon(":/icons/folder.png"));
    rootItem->setExpanded(true);
    connect(treeView,&QTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem *item, int column){
        qDebug() << "双击";
        folderItemWidget->clearTreeWidget();
        QString currentPath=item->text(6);
        qDebug() << "打开路径：" << currentPath;
        currentPath=currentPath.replace("//","/");
        folderItemWidget->setCurrentDirEdit(currentPath);
        sftpClient->opendir(currentPath);
    });

    connect(treeView,&QTreeWidget::itemActivated,[&](QTreeWidgetItem *item, int column){
        qDebug() << "回车";

    });

    treeView->setColumnCount(1);//设置列
    treeView->hideColumn(6);
    treeView->setHeaderLabels(QStringList()<<"文件名");

    layout->addWidget(treeView);
    folderItemWidget = new FolderItemWidget(this,sftpClient);
    layout->addWidget(folderItemWidget);

    layout->setStretch(0,1);
    layout->setStretch(1,4);
    setLayout(layout);
}

void SftpDialog::treeWidgetItemRefresh(QTreeWidgetItem* item){
    int count=item->childCount();
    for(int i=0;i<count;i++){
        item->removeChild(item->takeChild(0));
    }
    QString currentPath=item->text(6);
    item->setExpanded(true);
    sftpClient->opendir(currentPath);
}

void SftpDialog::closeEvent(QCloseEvent *event){
    sftpClient->stop();
    QTimer::singleShot(2000,[&](){
        delete sftpClient;
        delete this;
    });
}


SftpDialog::~SftpDialog()
{
    delete ui;

}
