#ifndef CONSOLE_H
#define CONSOLE_H

#include <QAbstractScrollArea>
#include "sshclient.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMutex>
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class Console;
}

class Console : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();
    SSHClient* sshClient;

private:
    Ui::Console *ui;
    QString data;
    QLineEdit* lineEdit=NULL;
    QVBoxLayout *layout;
    QMutex lock;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);

private slots:
    void readChannelData(const char data);
    void connectSuccess();
};

#endif // CONSOLE_H
