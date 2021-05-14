#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include "sshclient.h"
namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome();
    SSHClient* client;

private slots:
    void on_send_clicked();

private:
    Ui::Welcome *ui;
};

#endif // WELCOME_H
