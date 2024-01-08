#include "sftpdialog.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QtConcurrent>

#include "components/confirmdialog.h"
#include "fileinfo.h"

SftpDialog::SftpDialog(QWidget *parent, ConnectInfo connectInfo)
    : QWidget(parent) {
  this->connectInfo = connectInfo;
  //  setWindowTitle(connectInfo->name + "-文件管理");
  //  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  initUI();
  sftpConnect();
}

void SftpDialog::initUI() {
  hBoxLayout = new QHBoxLayout;
  treeView = new QTreeWidget(this);
  treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  treeView->sortByColumn(0, Qt::SortOrder::AscendingOrder);
  treeView->setItemsExpandable(true);
  QFont font;
  font.setPointSize(12);
  treeView->setFont(font);
  rootItem = new QTreeWidgetItem(treeView);
  rootItem->setText(0, "/");
  rootItem->setText(6, "/");
  rootItem->setIcon(0, QIcon(":/icons/folder.png"));
  rootItem->setExpanded(true);
  connect(treeView, &QTreeWidget::itemDoubleClicked, this,
          [=](QTreeWidgetItem *item, int column) {
            qDebug() << "双击";
            folderItemWidget->clearTreeWidget();
            QString currentPath = item->text(6);
            qDebug() << "打开路径：" << currentPath;
            currentPath = currentPath.replace("//", "/");
            folderItemWidget->setCurrentDirEdit(currentPath);
            sftpClient->asyncOpendir(currentPath);
          });

  connect(treeView, &QTreeWidget::itemActivated,
          [&](QTreeWidgetItem *item, int column) { qDebug() << "回车"; });

  treeView->setColumnCount(1); //设置列
  treeView->hideColumn(6);
  treeView->setHeaderLabels(QStringList() << "文件名");

  hBoxLayout->addWidget(treeView);
  // folderItemWidget = new FolderItemWidget(this, sftpClient);
  // hBoxLayout->addWidget(folderItemWidget);

  // hBoxLayout->setStretch(1, 4);
  setLayout(hBoxLayout);
}

void SftpDialog::sftpConnect() {
  if (sftpClient) {
    disconnect(sftpClient);
    sftpClient->stop();
    delete sftpClient;
    sftpClient = NULL;
  }
  if (folderItemWidget) {
    hBoxLayout->removeWidget(folderItemWidget);
  }

  sftpClient = new SFTPClient(connectInfo);

  folderItemWidget = new FolderItemWidget(this, sftpClient);
  hBoxLayout->addWidget(folderItemWidget);
  hBoxLayout->setStretch(1, 4);

  qDebug() << "SftpDialog ThreadId is" << QThread::currentThreadId();
  connect(sftpClient, &SFTPClient::disconnected, this, [=]() {
    AlertWindow *alertWindow = new AlertWindow(this, true);
    alertWindow->setTitleText("错误提示");
    alertWindow->setContentText("当前连接已断开\n是否重新连接");
    alertWindow->setConfirmButtonText("重连");
    connect(alertWindow, &AlertWindow::confirmEvent, this,
            [=]() { this->sftpConnect(); });
    alertWindow->show();
    this->installEventFilter(alertWindow);
  });
  connect(sftpClient, &SFTPClient::initSftpSessionSuccess, this, [=]() {
    qDebug() << "initSftpSessionSuccess ThreadId is"
             << QThread::currentThreadId();
    sftpClient->asyncOpendir(rootDir);
  });
  connect(sftpClient, &SFTPClient::errorMsg, this, [=](QString msg) {
    AlertWindow *alertWindow = new AlertWindow(this, true);
    alertWindow->setTitleText("错误提示");
    alertWindow->setContentText(msg);
    alertWindow->confirmButtonHide();
    alertWindow->setCancelButtonText("关闭");
    connect(alertWindow, &AlertWindow::cancelEvent, this,
            [=]() { this->close(); });
    alertWindow->show();
  });
  connect(sftpClient, &SFTPClient::successMsg, this,
          [=](QString msg) { QMessageBox::information(this, "提示", msg); });
  connect(sftpClient, &SFTPClient::opendirEvent, this,
          [=](QString currentPath) {
            if (currentPath == "/") {
              refresh = true;
              int count = rootItem->childCount();
              for (int i = 0; i < count; i++) {
                rootItem->removeChild(rootItem->takeChild(0));
              }
            } else {
              refresh = false;
            }
          });
  connect(sftpClient, &SFTPClient::opendirInfoCallBack, this,
          [=](QString currentPath, QString data) {
            if (!refresh) {
              return;
            }
            FileInfo_S info = parseBySftpData(data);
            if (info.fileName == "." || info.fileName == "..") {
              return;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(rootItem);
            item->setText(0, info.fileName);
            item->setText(1, info.fileSize);
            item->setText(2, info.fileNum);

            if (info.fileType == 1) {
              item->setIcon(0, QIcon(":/icons/folder.png"));
              item->setText(3, "目录");
            } else if (info.fileType == 2) {
              item->setIcon(0, QIcon(":/icons/file.png"));
              item->setText(3, "文件");
            } else if (info.fileType == 3) {
              item->setIcon(0, QIcon(":/icons/folder.png"));
              item->setText(3, "符号链接");
            }
            item->setText(4, info.permission);
            item->setText(5, info.userOrGroup);

            item->setText(6, currentPath + "/" + info.fileName);
            info.filePath = currentPath + "/" + info.fileName;
            item->setData(0, Qt::UserRole + 1, QVariant::fromValue(info));

            item->setText(7, info.updateTime);
          });
  sftpClient->start();
}

void SftpDialog::treeWidgetItemRefresh(QTreeWidgetItem *item) {
  int count = item->childCount();
  for (int i = 0; i < count; i++) {
    item->removeChild(item->takeChild(0));
  }
  QString currentPath = item->text(6);
  item->setExpanded(true);
  sftpClient->asyncOpendir(currentPath);
}

void SftpDialog::closeEvent(QCloseEvent *event) {
  sftpClient->stop();
  QTimer::singleShot(100, [&]() { delete this; });
}

SftpDialog::~SftpDialog() { delete sftpClient; }
