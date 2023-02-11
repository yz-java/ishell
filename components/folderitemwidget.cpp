#include "folderitemwidget.h"
#include <QPainter>
#include <QDebug>
#include "fileinfo.h"
#include "confirmdialog.h"
FolderItemWidget::FolderItemWidget(QWidget *parent,SFTPClient* sftpClient) : QWidget(parent)
{
    setAcceptDrops(true);
    this->sftpClient = sftpClient;
//    setAutoFillBackground(true);

    connect(sftpClient,&SFTPClient::opendirCallBack,this,[=](QString data){

        FileInfo_S info = parseBySftpData(data);
        if(info.fileName=="."||info.fileName==".."){
            return;
        }
        QTreeWidgetItem* item = new QTreeWidgetItem(treeView);

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
//        QString currentPath=currentItem->text(6);
        item->setText(6,currentDirPath+"/"+info.fileName);
        info.filePath=currentDirPath+"/"+info.fileName;
        item->setData(0,Qt::UserRole+1,QVariant::fromValue(info));

        item->setText(7,info.updateTime);
    });

    connect(sftpClient,&SFTPClient::fileUploadProcess,this,[&](int fileSize,int currentSize,float process){
        int p=process*100;
        progressBarMaster->setValue(p);
        p=QString::number(process,'f',4).mid(4,6).toInt();
        progressBarChild->setValue(p);
    });
    connect(sftpClient,&SFTPClient::fileUploadSuccess,this,[&](){
        progressBarMaster->setHidden(true);
        progressBarChild->setHidden(true);
        treeView->setEnabled(true);
        QMessageBox::information(this,"提示","上传完成");
    });

    connect(sftpClient,&SFTPClient::fileDownloadProcess,this,[&](int fileSize,int currentSize,float process){
        int p=process*100;
        progressBarMaster->setValue(p);
        p=QString::number(process,'f',4).mid(4,6).toInt();
        progressBarChild->setValue(p);
    });
    connect(sftpClient,&SFTPClient::fileDownloadSuccess,this,[&](){
        progressBarMaster->setHidden(true);
        progressBarChild->setHidden(true);
        treeView->setEnabled(true);
        QMessageBox::information(this,"提示","下载完成");
    });

    QVBoxLayout* vDirlayout=new QVBoxLayout(this);
    vDirlayout->setMargin(0);

    QHBoxLayout* titlelayout=new QHBoxLayout(this);
    currentDirEdit = new QLineEdit;
    currentDirEdit->setFixedHeight(30);
    titlelayout->addWidget(currentDirEdit);

    QPushButton* gotoButton = new QPushButton("跳转");
    connect(gotoButton,&QPushButton::clicked,this,[=](){
        QString currentDir = this->currentDirEdit->text().isEmpty()?"/":this->currentDirEdit->text();
        this->setCurrentDirEdit(currentDir);
        treeView->clear();
        sftpClient->opendir(currentDirPath);
    });
    gotoButton->setFixedHeight(30);
    titlelayout->addWidget(gotoButton);
    QPushButton* refreshButton = new QPushButton("刷新");
    connect(refreshButton,&QPushButton::clicked,this,[=](){
        treeView->clear();
        sftpClient->opendir(currentDirPath);
    });
    refreshButton->setFixedHeight(30);
    titlelayout->addWidget(refreshButton);

    QPushButton* newFolderButton = new QPushButton("新建目录");
    connect(newFolderButton,&QPushButton::clicked,this,[=](){
        this->createFolder();
    });
    newFolderButton->setFixedHeight(30);
    titlelayout->addWidget(newFolderButton);

    vDirlayout->addLayout(titlelayout);
    treeView = new QTreeWidget(this);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->sortByColumn(0,Qt::SortOrder::AscendingOrder);
    treeView->setSortingEnabled(true);
    treeView->setColumnCount(8);//设置列
    treeView->hideColumn(6);
    treeView->setColumnWidth(0,200);
    treeView->setHeaderLabels(QStringList()<<"文件名"<<"大小"<< "目录/链接个数" <<"类型"<<"权限"<<"用户/用户组"<< "父路径" <<"修改时间");
    QFont font;
    font.setPointSize(12);
    treeView->setFont(font);
//    treeView->setStyleSheet("QTreeWidget::item { height: 20px; }");

    //    connect(treeView,&QTreeWidget::itemClicked,[&](QTreeWidgetItem *item, int column){
//        qDebug() << "点击";
//    });

//    connect(treeView,&QTreeWidget::itemPressed,[&](QTreeWidgetItem *item, int column){
//        qDebug() << "itemPressed";
//    });
    connect(treeView,&QTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem *item, int column){
        qDebug() << "双击";
        QString currentPath=item->text(6);
        setCurrentDirEdit(currentPath);
        qDebug() << "打开路径：" << currentPath;
        treeView->clear();
//        treeView->update();
        sftpClient->opendir(currentPath);


    });

    connect(treeView,&QTreeWidget::itemActivated,[&](QTreeWidgetItem *item, int column){
        qDebug() << "回车";

    });
    connect(treeView,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(popMenu(const QPoint&)));
    vDirlayout->addWidget(treeView);

    progressBarMaster=new QProgressBar;
    progressBarMaster->setHidden(true);
    progressBarChild=new QProgressBar;
    progressBarChild->setHidden(true);
    vDirlayout->addWidget(progressBarMaster);
    vDirlayout->addWidget(progressBarChild);
    setLayout(vDirlayout);
}

void FolderItemWidget::setCurrentDirEdit(QString currentDir)
{
    this->currentDirPath=currentDir.replace("//","/");
    this->currentDirEdit->setText(currentDir);
}

void FolderItemWidget::clearTreeWidget()
{
    this->treeView->clear();
}

void FolderItemWidget::setCurrentDir(QString currentDir)
{
    this->currentDirPath=currentDir.replace("//","/");
}

void FolderItemWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void FolderItemWidget::dragMoveEvent(QDragMoveEvent *event)
{
     event->acceptProposedAction();
}

void FolderItemWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        for(QUrl url:urlList){
            QString filePath = url.toLocalFile();
            this->fileUpload(filePath);
        }
    }
}

void FolderItemWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void FolderItemWidget::popMenu(const QPoint &p)
{
    QTreeWidgetItem* curItem=treeView->currentItem();  //获取当前被点击的节点
    if(curItem==NULL||curItem->text(0)=="/"){
        return;
    }

    QMenu menu(treeView);
    FileInfo_S info=curItem->data(0,Qt::UserRole+1).value<FileInfo_S>();
    QAction* renameAction=new QAction("重命名",this);
    connect(renameAction, &QAction::triggered,this,[=](){
        ConfirmDialog* dialog=new ConfirmDialog(this,"请输入文件名称");
        dialog->show();
        connect(dialog,&ConfirmDialog::successEdit,[=](QString input){
            QString filePath=curItem->text(6);
            QFileInfo fileInfo(filePath);
            QString newFileName=input;
            if(info.fileType==2){
                newFileName+="."+fileInfo.suffix();
            }
            QString targetPathName = fileInfo.absolutePath()+"/"+newFileName;
            qDebug() << filePath << " to "<<targetPathName;
            bool status=sftpClient->rename(filePath,targetPathName);
            if(status){
                curItem->setText(0,newFileName);
                curItem->setText(6,targetPathName);

            }
        });
    });
    menu.addAction(renameAction);

    QAction* deleteAction=new QAction("删除",this);
    connect(deleteAction, &QAction::triggered,this,[&](){
        QString path=curItem->text(6);
        int result=QMessageBox::warning(this,"资源删除提示","是否删除"+info.fileName,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(result!=QMessageBox::Yes){
            return;
        }
        qDebug() << "删除 ==> " << path;
        bool status=sftpClient->rmByShell(path);
        if(status){
            delete curItem;
        }
    });
    menu.addAction(deleteAction);

    if(info.fileType==1||info.fileType==3){
//        QAction* refreshAction=new QAction("刷新",this);
//        connect(refreshAction, &QAction::triggered,this,[&](){
//            this->treeWidgetItemRefresh(curItem);
//        });
//        menu.addAction(refreshAction);
        QAction* mkdirAction=new QAction("新建文件夹",this);
        connect(mkdirAction, &QAction::triggered,this,[&](){
            createFolder();
        });
        menu.addAction(mkdirAction);

        QAction* uploadAction=new QAction("上传文件",this);
        connect(uploadAction, &QAction::triggered,this,[&](){
            this->fileUpload("");
        });
        menu.addAction(uploadAction);
    }

    if(info.fileType==2){
        QAction* downloadAction=new QAction("下载",this);
        connect(downloadAction, &QAction::triggered,this,[&](){
            fileDownload(curItem->text(6));
        });
        menu.addAction(downloadAction);
    }


    menu.exec(QCursor::pos());  //在当前鼠标位置显示
}

void FolderItemWidget::treeWidgetItemRefresh(QTreeWidgetItem* item){
//    FileInfo_S info=item->data(0,Qt::UserRole+1).value<FileInfo_S>();
    int count=item->childCount();
    for(int i=0;i<count;i++){
        item->removeChild(item->takeChild(0));
    }
    QString currentPath=item->text(6);
    item->setExpanded(true);
    sftpClient->opendir(currentPath);
}

void FolderItemWidget::fileUpload(QString filePath){
    if(filePath.isEmpty()){
        filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "",  tr("*"));
    }
    if(filePath.isEmpty()){
        return;
    }
    QFileInfo info(filePath);
    QString fileName=info.fileName();
    sftpClient->scpUpload(filePath,currentDirPath+"/"+fileName);
    progressBarMaster->setHidden(false);
    progressBarChild->setHidden(false);
    treeView->setEnabled(false);

}

void FolderItemWidget::fileDownload(QString remotePath){
    QFileDialog* dialog=new QFileDialog();
    dialog->setFileMode(QFileDialog::Directory);
    if(!dialog->exec()){
        return;
    }
    QString localPath = dialog->selectedFiles().at(0);
    qDebug() << "选择本地路径：" << localPath;
    QFileInfo info(remotePath);
    localPath += "/"+info.fileName();
    qDebug() << "localPath ==> " << localPath;
    QFileInfo localFileInfo(localPath);
    if(localFileInfo.exists()){
        int code=QMessageBox::warning(this,"警告","是否覆盖本地文件",QMessageBox::No|QMessageBox::Yes,QMessageBox::Yes);
        if(code==QMessageBox::No){
            return;
        }
    }
    sftpClient->scpDownload(remotePath,localPath);
    progressBarMaster->setHidden(false);
    progressBarChild->setHidden(false);
    treeView->setEnabled(false);
}

void FolderItemWidget::createFolder()
{
    ConfirmDialog* dialog=new ConfirmDialog(this,"请输入文件名称","新建");
    dialog->show();
    connect(dialog,&ConfirmDialog::successEdit,[=](QString input){
        bool status=sftpClient->mkdir(currentDirPath+"/"+input);
        if(status){
            treeView->clear();
            sftpClient->opendir(currentDirPath);
        }
        disconnect(dialog);
    });
}
