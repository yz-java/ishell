#include "vncproxyclient.h"

#include "websocketserver.h"
VncProxyClient::VncProxyClient(QString clientId, QString host, int port)
    : QObject(nullptr) {
  this->host = host;
  this->port = port;
  this->clientId = clientId;
  socket = new QTcpSocket(this);

  connect(socket, &QTcpSocket::readyRead, this, &VncProxyClient::readData);
  connect(socket, &QTcpSocket::connected, this, &VncProxyClient::connected);
  connect(socket, &QTcpSocket::disconnected, this,
          &VncProxyClient::disconnectFromHost);
  auto webSocketClient =
      WebSocketServer::getInstance()->getWebSocketClient(clientId);
  connect(webSocketClient, &QWebSocket::binaryMessageReceived, this,
          [=](const QByteArray& message) { emit writeData(message); });
  connect(this, &VncProxyClient::writeData, this,
          [=](QByteArray data) { socket->write(data); });
}

VncProxyClient::~VncProxyClient() {
  ;
  socket->disconnectFromHost();
}

void VncProxyClient::connectToHost() {
  socket->connectToHost(this->host, port);
}

void VncProxyClient::connected() {}

void VncProxyClient::readData() {
  QByteArray data = socket->readAll();
  WebSocketServer::getInstance()->sendData(clientId, data);
}

void VncProxyClient::disconnectFromHost() {
  emit disconnected();
  socket->disconnectFromHost();
}
