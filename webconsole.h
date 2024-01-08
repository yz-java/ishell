#ifndef WEBCONSOLE_H
#define WEBCONSOLE_H

#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>

#include "components/alertwindow.h"
#include "connectinfo.h"
#include "sshclient.h"
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class WebConsole;
}

class WebConsole : public QWidget {
  Q_OBJECT

 public:
  WebConsole(QWidget* parent, ConnectInfo* info);
  ~WebConsole();
  ConnectInfo connectInfo;
  SSHClient* sshClient = NULL;
  QWebEngineView* webView;
  QWebChannel* webChannel;

 protected:
  void closeEvent(QCloseEvent* event);
  void paintEvent(QPaintEvent* event);

 private:
  Ui::WebConsole* ui;
  void resizeEvent(QResizeEvent*);

  bool openChannelSeccess = false;
  int rows;
  int cols;
  QString clientId;

 public slots:
  void connectSuccess();
  void pageLoadFinished(bool flag);
  void channelDataHandle(QString data);

  // QWebChannel function
  void ssh2connect(const QString& jsMsg);
  void recieveJsMessage(const QString& jsMsg);
  void setChannelRequestPtySize(const QString& size);
};

#endif  // WEBCONSOLE_H
