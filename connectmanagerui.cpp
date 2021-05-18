#include "connectmanagerui.h"
#include "ui_connectmanagerui.h"
#include "connecteditui.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMovie>
#include "mylabel.h"
#include <QAction>
#include <QMenu>
#include <QHeaderView>
#include "db/connectdao.h"


ConnectManagerUI::ConnectManagerUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectManagerUI)
{
    ui->setupUi(this);

    setWindowTitle("连接管理器");
    setFixedSize(600, 300);
    QVBoxLayout* rootlayout=new QVBoxLayout();

    QHBoxLayout* hlayout=new QHBoxLayout();

    QVBoxLayout* vAddFolderlayout=new QVBoxLayout();
    MyLabel *folderlabel =new MyLabel();
    connect(folderlabel,&MyLabel::click,[=](){
        createNewGroup();
    });
    QPixmap folderpixmap(":/icons/folder.png");
    QSize sz(25, 25);
    folderpixmap = folderpixmap.scaled(sz, Qt::KeepAspectRatio);
    folderlabel->setPixmap(folderpixmap);
    folderlabel->show();
    MyLabel *grouplabel =new MyLabel("新建组");
    connect(grouplabel,&MyLabel::click,[=](){
        createNewGroup();
    });
    vAddFolderlayout->addWidget(folderlabel,0,Qt::AlignCenter);
    vAddFolderlayout->addWidget(grouplabel,0,Qt::AlignCenter);

    QVBoxLayout* vAddServerlayout=new QVBoxLayout();
    MyLabel *refreshIconlabel =new MyLabel();
    connect(refreshIconlabel,&MyLabel::click,[=](){
        this->refreshTreeWidget();
    });
    QPixmap refreshpixmap(":/icons/refresh.png");
    refreshpixmap = refreshpixmap.scaled(sz, Qt::KeepAspectRatio);
    refreshIconlabel->setPixmap(refreshpixmap);
    refreshIconlabel->show();
    MyLabel *refreshlabel =new MyLabel("刷新");
    connect(refreshlabel,&MyLabel::click,[=](){
        this->refreshTreeWidget();
    });
    vAddServerlayout->addWidget(refreshIconlabel,0,Qt::AlignCenter);
    vAddServerlayout->addWidget(refreshlabel,0,Qt::AlignCenter);

    hlayout->addLayout(vAddFolderlayout);
//    hlayout->addStretch(1);
    hlayout->addLayout(vAddServerlayout);

    rootlayout->addLayout(hlayout);

    treeView=new QTreeWidget(this);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView,&QTreeWidget::itemClicked,[&](QTreeWidgetItem *item, int column){
        qDebug() << "点击";
    });

    connect(treeView,&QTreeWidget::itemPressed,[&](QTreeWidgetItem *item, int column){
        qDebug() << "itemPressed";
    });
    connect(treeView,&QTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem *item, int column){
        qDebug() << "双击";
        ConnectInfo info = getConnectInfo(item);
        if(info.parentId!=0){
            emit openSSHConnect(info);
        }
    });

    connect(treeView,&QTreeWidget::itemActivated,[&](QTreeWidgetItem *item, int column){
        qDebug() << "回车";

    });

    connect(treeView,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(popMenu(const QPoint&)));

    treeView->setColumnWidth(0,200);
    treeView->setColumnWidth(1,150);
    treeView->setColumnCount(6);//设置列
    treeView->hideColumn(4);
    treeView->hideColumn(5);
    treeView->setHeaderLabels(QStringList()<<"名称"<<"用户名"<<"Host"<<"端口"<<"id"<<"parent_id");
    rootlayout->addWidget(treeView);
    this->setLayout(rootlayout);

    this->refreshTreeWidget();
}

void ConnectManagerUI::createNewGroup(){
    int id=ConnectDao::GetInstance()->addConnectInfo("新建文件夹");
    if(id>1){
        QTreeWidgetItem* group2 = new QTreeWidgetItem(treeView);
        group2->setText(0,"新建文件夹");
        group2->setIcon(0,QIcon(":/icons/folder.png"));
        group2->setText(4,QString::number(id));
    }

//    group2->setFlags(group2->flags() | Qt::ItemIsEditable);
}

void ConnectManagerUI::createNewServer(){
    QMessageBox::information(NULL,"单击","创建服务器",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
}

void ConnectManagerUI::popMenu(const QPoint& p){
    QTreeWidgetItem* curItem=treeView->currentItem();  //获取当前被点击的节点
        if(curItem==NULL){
            return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
        }

        QMenu menu(treeView);
        QString wellName = curItem->text(0);
        int id = curItem->text(4).toInt();
        ConnectInfo info = ConnectDao::GetInstance()->getConnectInfo(id);
        QAction* addconnect;
        if(info.parentId==0){
            addconnect=new QAction("新增连接",this);//新增连接
            connect(addconnect, &QAction::triggered,[&](){
                QString title="新增连接";
                ConnectEditUI *connectEditUI=new ConnectEditUI(this,title);
                connectEditUI->setConnectInfo(info);
                connectEditUI->show();
                connect(connectEditUI,&ConnectEditUI::addSuccess,[=](ConnectInfo info){
                    this->addChileItem(curItem,info);
                });
            });
            menu.addAction(addconnect);
        }
        QAction* connectWell;

        if(info.parentId!=0){
            connectWell=new QAction("连接",this);//连接
            connect(connectWell, &QAction::triggered,[&](){
                emit openSSHConnect(info);
            });
            menu.addAction(connectWell);


        }
        QAction* editWell=new QAction("编辑",this);
        connect(editWell, &QAction::triggered,[&](){
            if(info.parentId==0){
                ConfirmDialog* dialog=new ConfirmDialog(this,"请输入资源名称");
                dialog->show();
                connect(dialog,&ConfirmDialog::successEdit,[=](QString input){
                    int id=curItem->text(4).toInt();
                    this->updateConnectName(curItem,id,input);
                });
            }else{
                QString title=curItem->text(0);
                title+="-编辑";
                ConnectEditUI *connectEditUI=new ConnectEditUI(this,title);
                connectEditUI->setConnectInfo(info);
                connectEditUI->show();
                connect(connectEditUI,&ConnectEditUI::updateSuccess,[=](ConnectInfo info){
                    this->upadateChileItem(curItem,info);
                });
            }


        });
        menu.addAction(editWell);
        QAction* deleteWell;
        if(info.id!=1){
            deleteWell=new QAction("删除",this);//删除
            //在界面上删除该item
            connect(deleteWell, &QAction::triggered,this,[&](){

                if(info.parentId!=0){
                    ConnectDao::GetInstance()->deleteById(info.id);
                    delete  curItem;
                }else{
                    QMessageBox messageBox(QMessageBox::NoIcon,
                                              "数据删除警告", "删除该组下所有资源?",
                                              QMessageBox::Yes | QMessageBox::No, this);
                       int result=messageBox.exec();
                        if(result==QMessageBox::Yes){
                            ConnectDao::GetInstance()->deleteById(info.id);
                            ConnectDao::GetInstance()->deleteByParentId(info.id);
                            delete  curItem;
                        }
                        messageBox.close();
                }


            });
            menu.addAction(deleteWell);
        }

        menu.exec(QCursor::pos());  //在当前鼠标位置显示
}

void ConnectManagerUI::updateConnectName(QTreeWidgetItem* item,int id,QString name){
    bool result=ConnectDao::GetInstance()->updateConnectName(id,name);
    if(!result){
        QMessageBox::warning(this,"提示","名称修改失败",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        return;
    }
    item->setText(0,name);
}

void ConnectManagerUI::refreshList(QTreeWidgetItem* item){
    int id=item->text(4).toInt();
    QList<ConnectInfo> infos =ConnectDao::GetInstance()->getConnectInfos(id);
    for(auto info:infos){
        QTreeWidgetItem* child = new QTreeWidgetItem(rootItem);
        child->setText(0,info.name);
        child->setIcon(0,QIcon(":/icons/server.png"));
        child->setText(1,info.username);
        child->setText(2,info.hostName);
        child->setText(3,QString::number(info.port));
        child->setText(4,QString::number(info.id));
//        child->setFlags(child->flags() | Qt::ItemIsEditable);
        rootItem->addChild(child);
    }
}


void ConnectManagerUI::addChileItem(QTreeWidgetItem* item,ConnectInfo info){
    QTreeWidgetItem* child = new QTreeWidgetItem(item);
    child->setText(0,info.name);
    child->setIcon(0,QIcon(":/icons/server.png"));
    child->setText(1,info.username);
    child->setText(2,info.hostName);
    child->setText(3,QString::number(info.port));
    child->setText(4,QString::number(info.id));
    item->addChild(child);
}

void ConnectManagerUI::upadateChileItem(QTreeWidgetItem* item,ConnectInfo info){
    item->setText(0,info.name);
    item->setIcon(0,QIcon(":/icons/server.png"));
    item->setText(1,info.username);
    item->setText(2,info.hostName);
    item->setText(3,QString::number(info.port));
    item->setText(4,QString::number(info.id));
}

ConnectInfo ConnectManagerUI::getConnectInfo(QTreeWidgetItem* item){
    int id = item->text(4).toInt();
    ConnectInfo info = ConnectDao::GetInstance()->getConnectInfo(id);
    return info;
}

void ConnectManagerUI::refreshTreeWidget(){
    treeView->clear();
    this->connectInfos = ConnectDao::GetInstance()->getConnectInfos(0);
    for (ConnectInfo info:this->connectInfos ) {
        rootItem = new QTreeWidgetItem(treeView);
        rootItem->setText(0,info.name);
        rootItem->setIcon(0,QIcon(":/icons/folder.png"));
        rootItem->setText(4,QString::number(info.id));
        refreshList(rootItem);
    }
}

ConnectManagerUI::~ConnectManagerUI()
{
    delete ui;
}
