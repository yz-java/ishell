#ifndef CONNECTEDITUI_H
#define CONNECTEDITUI_H

#include <QDialog>
#include <connectinfo.h>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class ConnectEditUI;
}

class ConnectEditUI : public QDialog
{
    Q_OBJECT

public:
    ConnectEditUI(QWidget *parent = nullptr,QString title="");
    ~ConnectEditUI();

    ConnectInfo info;

    void refreshPage();
    void setConnectInfo(ConnectInfo info);

private slots:
    void on_authType_currentIndexChanged(int index);

    void on_selectPrivateKeyFileButton_clicked();

    void on_save_clicked();

    void on_selectPublicKeyFileButton_clicked();

    void on_cancel_clicked();

private:
    Ui::ConnectEditUI *ui;

signals:
    void addSuccess(ConnectInfo info);

    void updateSuccess(ConnectInfo info);
};

#endif // CONNECTEDITUI_H
