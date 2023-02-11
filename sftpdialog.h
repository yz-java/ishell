#ifndef SFTPDIALOG_H
#define SFTPDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "sftpclient.h"
#include <QProgressBar>
#include "components/folderitemwidget.h"
namespace Ui {
class SftpDialog;
}

class SftpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SftpDialog(QWidget *parent = nullptr);
    SftpDialog(QWidget *parent = nullptr,ConnectInfo* connectInfo=nullptr);
    ~SftpDialog();
    ConnectInfo* connectInfo;
    SFTPClient* sftpClient;

protected:
    void closeEvent(QCloseEvent *event);


private:
    Ui::SftpDialog *ui;
    QTreeWidget* treeView;
    QTreeWidgetItem* rootItem;
    FolderItemWidget* folderItemWidget;

    bool refresh = false;

    void initUI();

    QString rootDir="/";

    void treeWidgetItemRefresh(QTreeWidgetItem* item);

};

#endif // SFTPDIALOG_H
