#include "websocketserver.h"

#include <QUrlQuery>

QMap<QString, QWebSocket*> WebSocketServer::clientMap;
WebSocketServer* WebSocketServer::instance = nullptr;
std::mutex WebSocketServer::mutex;

WebSocketServer::WebSocketServer(QObject* parent) : QObject(parent) {}

void WebSocketServer::start() {
  qDebug() << "WebSocketServer ThreadId is" << QThread::currentThreadId();
  server = new QWebSocketServer("server", QWebSocketServer::NonSecureMode);
  if (server->isListening()) {
    return;
  }
  if (server->listen(QHostAddress::Any, 0)) {
    qDebug() << "WebSocketServer port is" << server->serverPort();
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    connect(this, SIGNAL(sendData(QString, QByteArray)), this,
            SLOT(writeData(QString, QByteArray)));
    connect(this, SIGNAL(sendQStringData(QString, QString)), this,
            SLOT(writeQStringData(QString, QString)));
  }
}

quint16 WebSocketServer::getWebSocketServerPort() {
  return server->serverPort();
}

QWebSocket* WebSocketServer::getWebSocketClient(QString clientId) {
  return clientMap[clientId];
}

void WebSocketServer::onNewConnection() {
  QWebSocket* socket = server->nextPendingConnection();
  QString clientId = "";
  QString peerName = socket->requestUrl().toString();
  QUrl url(peerName);
  QUrlQuery query(url.query());
  QList<QPair<QString, QString>> params = query.queryItems();
  for (QPair<QString, QString> param : params) {
    QString paramName = param.first;
    if (paramName == "clientId") {
      clientId = param.second;
    }
    // qDebug() << "Parameter:" << paramName << "Value:" << param.second;
  }

  connect(socket, &QWebSocket::disconnected, this, [=]() {
    qDebug() << "QWebSocket client disconnected " << clientId;
    clientMap.remove(clientId);
  });
  // qDebug() << "clientId is " << clientId;
  qDebug() << "onNewConnection ThreadId is" << QThread::currentThreadId();
  clientMap[clientId] = socket;
  emit connectSuccess(clientId);
}

void WebSocketServer::writeData(QString clientId, QByteArray data) {
  //  qDebug() << "WebSocketServer writeData ThreadId is"
  //           << QThread::currentThreadId();
  auto client = clientMap[clientId];
  if (client) {
    client->sendBinaryMessage(data);
  }
}

void WebSocketServer::writeQStringData(QString clientId, QString data) {
  auto client = clientMap[clientId];
  if (client) {
    client->sendTextMessage(data);
  }
}
