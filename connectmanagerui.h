#ifndef CONNECTMANAGERUI_H
#define CONNECTMANAGERUI_H

#include <QDialog>
#include <QTreeView>
#include <QTreeWidgetItem>

#include "backupandrecoverydialog.h"
#include "common.h"
#include "components/confirmdialog.h"
#include "connectinfo.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class ConnectManagerUI;
}

class ConnectManagerUI : public QWidget {
  Q_OBJECT

 public:
  explicit ConnectManagerUI(QWidget* parent = nullptr);
  ~ConnectManagerUI();
  void createNewGroup();
  void createNewServer();

 private:
  //  Ui::ConnectManagerUI* ui;
  QTreeWidgetItem* rootItem;
  QTreeWidget* treeView;
  BackupAndRecoveryDialog* backupAndRecoveryDialog;

  QList<ConnectInfo> connectInfos;

  ConnectInfo copyInfo;

  void updateConnectName(QTreeWidgetItem* item, int id, QString name);

  void refreshList(QTreeWidgetItem* item);

  void addChileItem(QTreeWidgetItem* item, ConnectInfo info);

  void upadateChileItem(QTreeWidgetItem* item, ConnectInfo info);

  ConnectInfo getConnectInfo(QTreeWidgetItem* item);

  void refreshTreeWidget();

 private slots:
  void popMenu(const QPoint& p);

 signals:
  void openSSHConnect(ConnectInfo connectInfo);

  void openVNCConnect(ConnectInfo connectInfo);

  void openRDPConnect(ConnectInfo connectInfo);

  void openSFTPConnect(ConnectInfo connectInfo);
};

#endif  // CONNECTMANAGERUI_H
