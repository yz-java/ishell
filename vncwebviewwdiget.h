#ifndef VNCWEBVIEWWDIGET_H
#define VNCWEBVIEWWDIGET_H

#include <QResizeEvent>
#include <QThread>
#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>

#include "components/alertwindow.h"
#include "connectinfo.h"
#include "vncproxyclient.h"

class VncWebViewWdiget : public QWidget {
  Q_OBJECT
 public:
  VncWebViewWdiget(QWidget* parent, ConnectInfo connInfo);
  ~VncWebViewWdiget();

 private:
  QString clientId;
  ConnectInfo connectInfo;
  QWebEngineView* webView;
  QWebChannel* webChannel;
  VncProxyClient* proxyClient;
  QThread* proxyClientThread = NULL;
  void resizeEvent(QResizeEvent*) override;

 public slots:
  void pageLoadFinished(bool flag);
  void connectSuccess();
};

#endif  // VNCWEBVIEWWDIGET_H
