﻿#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include "sshclient.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();
    SSHClient* sshClient;

private:
    Ui::Console *ui;
    void resizeEvent(QResizeEvent *);

private slots:
    void readChannelData(const char* data);
    void connectSuccess();
};

#endif // CONSOLE_H
