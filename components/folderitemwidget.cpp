#include "folderitemwidget.h"

#include <QDebug>
#include <QMenu>
#include <QPainter>

#include "fileinfo.h"
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif
FolderItemWidget::FolderItemWidget(QWidget *parent, SFTPClient *sftpClient)
    : QWidget(parent) {
  setAcceptDrops(true);
  this->sftpClient = sftpClient;
  //    setAutoFillBackground(true);
  connect(sftpClient, &SFTPClient::opendirCallBack, this, [=](QString data) {
    FileInfo_S info = parseBySftpData(data);
    if (info.fileName == "." || info.fileName == "..") {
      return;
    }
    QTreeWidgetItem *item = new QTreeWidgetItem(treeView);

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
    //        QString currentPath=currentItem->text(6);
    item->setText(6, currentDirPath + "/" + info.fileName);
    info.filePath = currentDirPath + "/" + info.fileName;
    item->setData(0, Qt::UserRole + 1, QVariant::fromValue(info));

    item->setText(7, info.updateTime);
  });

  connect(sftpClient, &SFTPClient::fileUploadProcess, this,
          [&](int fileSize, int currentSize, float process) {
            int p = process * 100;
            progressBarMaster->setValue(p);
            p = QString::number(process, 'f', 4).mid(4, 6).toInt();
            progressBarChild->setValue(p);
          });
  connect(sftpClient, &SFTPClient::fileUploadSuccess, this, [=]() {
    progressBarMaster->setHidden(true);
    progressBarChild->setHidden(true);
    treeView->setEnabled(true);
    QMessageBox::information(this, "提示", "上传完成");
    treeView->clear();
    sftpClient->asyncOpendir(currentDirPath);
  });

  connect(sftpClient, &SFTPClient::fileDownloadProcess, this,
          [&](int fileSize, int currentSize, float process) {
            int p = process * 100;
            progressBarMaster->setValue(p);
            p = QString::number(process, 'f', 4).mid(4, 6).toInt();
            progressBarChild->setValue(p);
          });
  connect(sftpClient, &SFTPClient::fileDownloadSuccess, this, [&]() {
    progressBarMaster->setHidden(true);
    progressBarChild->setHidden(true);
    treeView->setEnabled(true);
    QMessageBox::information(this, "提示", "下载完成");
  });

  QVBoxLayout *vDirlayout = new QVBoxLayout(this);
  vDirlayout->setMargin(0);

  QHBoxLayout *titlelayout = new QHBoxLayout(this);

  QPushButton *returnButton = new QPushButton("返回上一级");
  connect(returnButton, &QPushButton::clicked, this, [=]() {
    QString currentDirStr = this->currentDirEdit->text();
    if (currentDirStr != "/") {
      currentDirStr.remove(
          currentDirStr.lastIndexOf('/'),
          currentDirStr.length() - currentDirStr.lastIndexOf('/'));
    }
    if (currentDirStr.isEmpty()) {
      currentDirStr = "/";
    }
    this->setCurrentDirEdit(currentDirStr);
    treeView->clear();
    sftpClient->asyncOpendir(currentDirPath);
  });
  returnButton->setFixedHeight(30);
  titlelayout->addWidget(returnButton);

  currentDirEdit = new QLineEdit;
  currentDirEdit->setFixedHeight(30);
  connect(currentDirEdit, &QLineEdit::returnPressed, this,
          &FolderItemWidget::jumpDir);
  titlelayout->addWidget(currentDirEdit);

  QPushButton *gotoButton = new QPushButton("跳转");
  connect(gotoButton, &QPushButton::clicked, this, &FolderItemWidget::jumpDir);
  gotoButton->setFixedHeight(30);
  gotoButton->setShortcut(Qt::Key_Escape);
  gotoButton->setDefault(true);
  titlelayout->addWidget(gotoButton);
  //  QPushButton *refreshButton = new QPushButton("刷新");
  //  connect(refreshButton, &QPushButton::clicked, this, [=]() {
  //    treeView->clear();
  //    sftpClient->asyncOpendir(currentDirPath);
  //  });
  //  refreshButton->setFixedHeight(30);
  //  titlelayout->addWidget(refreshButton);

  //  QPushButton *newFolderButton = new QPushButton("新建目录");
  //  connect(newFolderButton, &QPushButton::clicked, this,
  //          [=]() { this->createFolder(); });
  //  newFolderButton->setFixedHeight(30);
  //  titlelayout->addWidget(newFolderButton);

  vDirlayout->addLayout(titlelayout);
  treeView = new QTreeWidget(this);
  treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  treeView->sortByColumn(0, Qt::SortOrder::AscendingOrder);
  treeView->setSortingEnabled(true);
  treeView->setColumnCount(8);  //设置列
  treeView->hideColumn(6);
  treeView->setColumnWidth(0, 200);
  treeView->setHeaderLabels(QStringList() << "文件名"
                                          << "大小"
                                          << "目录/链接个数"
                                          << "类型"
                                          << "权限"
                                          << "用户/用户组"
                                          << "父路径"
                                          << "修改时间");
  QFont font;
  font.setPointSize(12);
  treeView->setFont(font);
  treeView->installEventFilter(this);
  treeView->setSelectionMode(
      QAbstractItemView::SelectionMode::ExtendedSelection);
  connect(treeView, &QTreeWidget::itemDoubleClicked, this,
          [=](QTreeWidgetItem *item, int column) {
            qDebug() << "双击";
            QString currentPath = item->text(6);
            setCurrentDirEdit(currentPath);
            qDebug() << "打开路径：" << currentPath;
            treeView->clear();
            //        treeView->update();
            sftpClient->asyncOpendir(currentPath);
          });

  connect(treeView, &QTreeWidget::itemActivated,
          [&](QTreeWidgetItem *item, int column) { qDebug() << "回车"; });
  connect(treeView, &QTreeWidget::itemPressed, this,
          [=](QTreeWidgetItem *item, int column) {
            if (item == NULL) {
              treeView->clearSelection();
            }
          });
  connect(treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this,
          SLOT(popMenu(const QPoint &)));
  vDirlayout->addWidget(treeView);

  progressBarMaster = new QProgressBar;
  progressBarMaster->setHidden(true);
  progressBarChild = new QProgressBar;
  progressBarChild->setHidden(true);
  vDirlayout->addWidget(progressBarMaster);
  vDirlayout->addWidget(progressBarChild);
  setLayout(vDirlayout);
  confirmDialog = new ConfirmDialog(this);
  connect(
      confirmDialog, &ConfirmDialog::confirmEditEvent, this,
      [=](QString newName) {
        if (fileOption == FileOption::RENAME) {
          QTreeWidgetItem *item = treeView->currentItem();
          FileInfo_S info = item->data(0, Qt::UserRole + 1).value<FileInfo_S>();
          QString filePath = item->text(6);
          QFileInfo fileInfo(filePath);
          QString newFileName = newName;
          if (info.fileType == 2) {
            newFileName += "." + fileInfo.suffix();
          }
          QString targetPathName = fileInfo.absolutePath() + "/" + newFileName;
          qDebug() << filePath << " to " << targetPathName;
          bool status = sftpClient->rename(filePath, targetPathName);
          if (status) {
            item->setText(0, newFileName);
            item->setText(6, targetPathName);
          }
        }
        if (fileOption == FileOption::MKDIR) {
          bool status = sftpClient->mkdir(currentDirPath + "/" + newName);
          if (status) {
            treeView->clear();
            sftpClient->asyncOpendir(currentDirPath);
          }
        }
        fileOption = FileOption::DEFAULT;
      });
}

FolderItemWidget::~FolderItemWidget() {}

void FolderItemWidget::setCurrentDirEdit(QString currentDir) {
  this->currentDirPath = currentDir.replace("//", "/");
  this->currentDirEdit->setText(currentDir);
}

void FolderItemWidget::clearTreeWidget() { this->treeView->clear(); }

void FolderItemWidget::setCurrentDir(QString currentDir) {
  this->currentDirPath = currentDir.replace("//", "/");
}

void FolderItemWidget::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

void FolderItemWidget::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
}

void FolderItemWidget::dropEvent(QDropEvent *event) {
  const QMimeData *mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    QList<QUrl> urlList = mimeData->urls();
    for (QUrl url : urlList) {
      QString filePath = url.toLocalFile();
      this->fileUpload(filePath);
    }
  }
}

void FolderItemWidget::paintEvent(QPaintEvent *event) { Q_UNUSED(event); }

void FolderItemWidget::popMenu(const QPoint &p) {
  QTreeWidgetItem *curItem = treeView->currentItem();  //获取当前被点击的节点

  QMenu menu(treeView);

  QList<QTreeWidgetItem *> items = treeView->selectedItems();
  if (items.size() > 1) {
    QAction *deleteAction = new QAction("删除", this);
    connect(deleteAction, &QAction::triggered, this, [&]() {
      QString content = "";
      for (QTreeWidgetItem *item : items) {
        FileInfo_S info = item->data(0, Qt::UserRole + 1).value<FileInfo_S>();
        content += info.fileName + "\n";
      }
      int result = QMessageBox::warning(
          this, "资源删除提示", content + "是否删除",
          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
      if (result != QMessageBox::Yes) {
        return;
      }
      for (QTreeWidgetItem *item : items) {
        FileInfo_S info = item->data(0, Qt::UserRole + 1).value<FileInfo_S>();
        QString path = item->text(6);
        qDebug() << "删除 ==> " << path;
        bool status = false;
        if (info.fileType == 2) {
          status = sftpClient->removeFile(path);
        }
        if (info.fileType == 1) {
          status = sftpClient->rmdir(path);
        }
        if (status) {
          delete item;
        }
      }
    });
    menu.addAction(deleteAction);
    goto END;
  }
  if (curItem == NULL) {
    QAction *refreshAction = new QAction("刷新", this);
    connect(refreshAction, &QAction::triggered, this, [=]() {
      treeView->clear();
      sftpClient->asyncOpendir(currentDirPath);
    });
    menu.addAction(refreshAction);
    QAction *mkdirAction = new QAction("新建文件夹", this);
    connect(mkdirAction, &QAction::triggered, this, [&]() { createFolder(); });
    menu.addAction(mkdirAction);
    QAction *uploadAction = new QAction("上传文件", this);
    connect(uploadAction, &QAction::triggered, this,
            [&]() { this->fileUpload(""); });
    menu.addAction(uploadAction);
  } else {
    FileInfo_S info = curItem->data(0, Qt::UserRole + 1).value<FileInfo_S>();
    QAction *renameAction = new QAction("重命名", this);
    connect(renameAction, &QAction::triggered, this, [=]() {
      QString filePath = curItem->text(6);
      QFileInfo fileInfo(filePath);
      confirmDialog->setTitleText("请输入文件名");
      confirmDialog->setEditText(fileInfo.baseName());
      confirmDialog->setOkButtonName("修改");
      fileOption = FileOption::RENAME;
      confirmDialog->show();
    });
    menu.addAction(renameAction);

    QAction *deleteAction = new QAction("删除", this);
    connect(deleteAction, &QAction::triggered, this, [&]() {
      QString path = curItem->text(6);
      int result = QMessageBox::warning(
          this, "资源删除提示", "是否删除" + info.fileName,
          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
      if (result != QMessageBox::Yes) {
        return;
      }
      qDebug() << "删除 ==> " << path;
      bool status = false;
      if (info.fileType == 2) {
        status = sftpClient->removeFile(path);
      }
      if (info.fileType == 1) {
        status = sftpClient->rmdir(path);
      }
      if (status) {
        delete curItem;
      }
    });
    menu.addAction(deleteAction);

    //    if (info.fileType == 1 || info.fileType == 3) {

    //    }

    if (info.fileType == 2) {
      QAction *downloadAction = new QAction("下载", this);
      connect(downloadAction, &QAction::triggered, this,
              [&]() { fileDownload(curItem->text(6)); });
      menu.addAction(downloadAction);
    }
  }
END:
  menu.exec(QCursor::pos());  //在当前鼠标位置显示
}

void FolderItemWidget::treeWidgetItemRefresh(QTreeWidgetItem *item) {
  //    FileInfo_S info=item->data(0,Qt::UserRole+1).value<FileInfo_S>();
  int count = item->childCount();
  for (int i = 0; i < count; i++) {
    item->removeChild(item->takeChild(0));
  }
  QString currentPath = item->text(6);
  item->setExpanded(true);
  sftpClient->asyncOpendir(currentPath);
}

void FolderItemWidget::fileUpload(QString filePath) {
  if (filePath.isEmpty()) {
    filePath = QFileDialog::getOpenFileName(
        this, tr("选择文件"), "", tr("*"), 0, QFileDialog::DontUseNativeDialog);
  }
  if (filePath.isEmpty()) {
    return;
  }
  QFileInfo info(filePath);
  QString fileName = info.fileName();
  sftpClient->asyncScpUpload(filePath, currentDirPath + "/" + fileName);
  progressBarMaster->setHidden(false);
  progressBarChild->setHidden(false);
  treeView->setEnabled(false);
}

void FolderItemWidget::fileDownload(QString remotePath) {
  QFileDialog *dialog = new QFileDialog();
  dialog->setFileMode(QFileDialog::Directory);
  if (!dialog->exec()) {
    return;
  }
  QString localPath = dialog->selectedFiles().at(0);
  qDebug() << "选择本地路径：" << localPath;
  QFileInfo info(remotePath);
  localPath += "/" + info.fileName();
  qDebug() << "localPath ==> " << localPath;
  QFileInfo localFileInfo(localPath);
  if (localFileInfo.exists()) {
    int code = QMessageBox::warning(this, "警告", "是否覆盖本地文件",
                                    QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);
    if (code == QMessageBox::No) {
      return;
    }
  }
  sftpClient->asyncScpDownload(remotePath, localPath);
  progressBarMaster->setHidden(false);
  progressBarChild->setHidden(false);
  treeView->setEnabled(false);
}

void FolderItemWidget::createFolder() {
  confirmDialog->setTitleText("请输入文件名");
  confirmDialog->setEditText("");
  confirmDialog->setOkButtonName("新建");
  fileOption = FileOption::MKDIR;
  confirmDialog->show();
}

void FolderItemWidget::jumpDir() {
  QString currentDir = this->currentDirEdit->text().isEmpty()
                           ? "/"
                           : this->currentDirEdit->text();
  this->setCurrentDirEdit(currentDir);
  treeView->clear();
  sftpClient->asyncOpendir(currentDirPath);
}

bool FolderItemWidget::eventFilter(QObject *obj, QEvent *e) {
  QMouseEvent *event = static_cast<QMouseEvent *>(e);
  if (e->type() == QEvent::KeyPress && ((QKeyEvent *)e)->key() == Qt::Key_F5) {
    treeView->clear();
    sftpClient->asyncOpendir(currentDirPath);
    return QWidget::eventFilter(obj, event);
  }
  if (e->type() == QEvent::KeyPress && ((QKeyEvent *)e)->key() == Qt::Key_F2) {
    QList<QTreeWidgetItem *> items = treeView->selectedItems();
    if (items.size() == 1) {
      QTreeWidgetItem *item = items.at(0);
      QString filePath = item->text(6);
      QFileInfo fileInfo(filePath);
      confirmDialog->setTitleText("请输入文件名");
      confirmDialog->setEditText(fileInfo.baseName());
      confirmDialog->setOkButtonName("修改");
      fileOption = FileOption::RENAME;
      confirmDialog->show();
    }
    return QWidget::eventFilter(obj, event);
  }
  if (event == nullptr) {
    return QWidget::eventFilter(obj, event);
  }
  QPoint point = event->pos();
  QTreeWidgetItem *item = treeView->itemAt(point);
  if (item == nullptr) {
    treeView->setCurrentItem(NULL);
  }
  return QWidget::eventFilter(obj, event);
}
