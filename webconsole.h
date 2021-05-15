#ifndef WEBCONSOLE_H
#define WEBCONSOLE_H

#include <QWidget>
#include <QWebEngineView>
#include "sshclient.h"
#include <QWebChannel>
#include "connectinfo.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class WebConsole;
}

class WebConsole : public QWidget
{
    Q_OBJECT

public:
    WebConsole(QWidget *parent,ConnectInfo* info);
    ~WebConsole();
    ConnectInfo connectInfo;
    SSHClient* sshClient;
    QWebEngineView* webView;
    QWebChannel *webChannel;



private:
    Ui::WebConsole *ui;
    void resizeEvent(QResizeEvent *);


public slots:
    void connectSuccess();
    void pageLoadFinished(bool flag);
    void recieveJsMessage(const QString& jsMsg);
};

#endif // WEBCONSOLE_H
