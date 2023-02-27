#ifndef FOLDERITEMWIDGET_H
#define FOLDERITEMWIDGET_H

#include <QtWidgets>
#include <QProgressBar>
#include "sftpclient.h"
#include <QTableView>
class FolderItemWidget : public QWidget
{
    Q_OBJECT
public:
    FolderItemWidget(QWidget *parent,SFTPClient* sftpClient);

    void setCurrentDirEdit(QString currentDir);

    void clearTreeWidget();

private:
    QProgressBar* progressBarMaster;
    QProgressBar* progressBarChild;
    QTreeWidget* treeView;
    QLineEdit* currentDirEdit;
    QString currentDirPath = "/";
    SFTPClient* sftpClient;

    void setCurrentDir(QString currentDir);

    void treeWidgetItemRefresh(QTreeWidgetItem* item);

    void fileUpload(QString filePath);

    void fileDownload(QString remotePath);

    void createFolder();

protected:
//    void mousePressEvent(QMouseEvent *event);      // 鼠标按下事件
    void dragEnterEvent(QDragEnterEvent *event);   // 拖动进入事件
    void dragMoveEvent(QDragMoveEvent *event);     // 拖动事件
    void dropEvent(QDropEvent *event);             // 放下事件
    void paintEvent(QPaintEvent *event);
private slots:
    void popMenu(const QPoint& p);

signals:

};

#endif // FOLDERITEMWIDGET_H