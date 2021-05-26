#ifndef SFTPDIALOG_H
#define SFTPDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "sftpclient.h"
#include <QProgressBar>
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
    QTreeWidgetItem* currentItem;
    void initUI();
    QString rootDir="/";
    QProgressBar* progressBarMaster;
    QProgressBar* progressBarChild;


    void treeWidgetItemRefresh(QTreeWidgetItem* item);

    void fileUpload(QTreeWidgetItem* item);

    void fileDownload(QString remotePath);

private slots:
    void popMenu(const QPoint& p);
};

#endif // SFTPDIALOG_H
