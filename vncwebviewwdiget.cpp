#include "vncwebviewwdiget.h"

#include <QFile>

#include "websocketserver.h"
VncWebViewWdiget::VncWebViewWdiget(QWidget* parent, ConnectInfo connInfo)
    : QWidget(parent) {
  this->connectInfo = connInfo;
  qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
  qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
  setAttribute(Qt::WA_DeleteOnClose);

  clientId = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

  webChannel = new QWebChannel(this);
  webChannel->registerObject(QStringLiteral("core"), this);

  webView = new QWebEngineView(this);
  webView->page()->setWebChannel(webChannel);
  webView->resize(parent->size());

  QFile file(":/html/novnc/index.html");
  if (!file.open(QIODevice::ReadOnly)) {
    return;
  }
  QString htmlData = file.readAll().constData();
  webView->setHtml(htmlData);

  //  webView->setUrl(QUrl("http://192.168.0.110:8080/"));
  webView->show();
  connect(webView, SIGNAL(loadFinished(bool)), this,
          SLOT(pageLoadFinished(bool)));
  connect(WebSocketServer::getInstance(), &WebSocketServer::connectSuccess,
          this, [=](QString clientId) {
            if (clientId != this->clientId) {
              return;
            }
            proxyClientThread = new QThread;
            proxyClient = new VncProxyClient(clientId, connectInfo.hostName,
                                             connectInfo.vncPort);
            proxyClient->moveToThread(proxyClientThread);
            connect(proxyClientThread, &QThread::started, proxyClient,
                    &VncProxyClient::connectToHost);
            connect(proxyClientThread, &QThread::finished, proxyClient,
                    &VncProxyClient::deleteLater);
            proxyClientThread->start();
          });
}

VncWebViewWdiget::~VncWebViewWdiget() {
  disconnect(this);
  if (proxyClientThread) {
    proxyClientThread->quit();
    proxyClientThread->wait();
  }

  delete webChannel;
  webView->close();
  delete webView;
}

void VncWebViewWdiget::resizeEvent(QResizeEvent* event) {
  ;
  webView->resize(event->size());
}

void VncWebViewWdiget::pageLoadFinished(bool flag) {
  webView->page()->runJavaScript(
      "setWebSocketServerPort(" +
      QString::number(
          WebSocketServer::getInstance()->getWebSocketServerPort()) +
      ")");
  webView->page()->runJavaScript("setPassword('" + connectInfo.vncPassword +
                                 "')");
  webView->page()->runJavaScript("setClientId(" + clientId + ")");
}

void VncWebViewWdiget::connectSuccess() {}
