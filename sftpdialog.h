#ifndef SFTPDIALOG_H
#define SFTPDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QTreeWidgetItem>

#include "components/alertwindow.h"
#include "components/folderitemwidget.h"
#include "sftpclient.h"

class SftpDialog : public QWidget {
  Q_OBJECT

public:
  explicit SftpDialog(QWidget *parent, ConnectInfo connectInfo);
  ~SftpDialog();
  ConnectInfo connectInfo;
  SFTPClient *sftpClient = NULL;

protected:
  void closeEvent(QCloseEvent *event);

private:
  QHBoxLayout *hBoxLayout = NULL;
  QTreeWidget *treeView;
  QTreeWidgetItem *rootItem;
  FolderItemWidget *folderItemWidget = NULL;
  QThread *thread;

  bool refresh = false;

  void initUI();

  QString rootDir = "/";

  void sftpConnect();

  void treeWidgetItemRefresh(QTreeWidgetItem *item);
};

#endif // SFTPDIALOG_H
