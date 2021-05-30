#ifndef CONNECTMANAGERUI_H
#define CONNECTMANAGERUI_H

#include <QDialog>
#include "common.h"
#include <QTreeWidgetItem>
#include <QTreeView>
#include "connectinfo.h"
#include "components/confirmdialog.h"
#include "backupandrecoverydialog.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class ConnectManagerUI;
}

class ConnectManagerUI : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectManagerUI(QWidget *parent = nullptr);
    ~ConnectManagerUI();
    void createNewGroup();
    void createNewServer();

private:
    Ui::ConnectManagerUI *ui;
    QTreeWidgetItem* rootItem;
    QTreeWidget* treeView;
    BackupAndRecoveryDialog* backupAndRecoveryDialog;

    QList<ConnectInfo> connectInfos;

    void updateConnectName(QTreeWidgetItem* item,int id,QString name);

    void refreshList(QTreeWidgetItem* item);

    void addChileItem(QTreeWidgetItem* item,ConnectInfo info);

    void upadateChileItem(QTreeWidgetItem* item,ConnectInfo info);

    ConnectInfo getConnectInfo(QTreeWidgetItem* item);

    void refreshTreeWidget();

private slots:
    void popMenu(const QPoint& p);


signals:
    void openSSHConnect(ConnectInfo connectInfo);

};

#endif // CONNECTMANAGERUI_H
