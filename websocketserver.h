#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H
#include <QThread>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <mutex>
class WebSocketServer : public QObject {
  Q_OBJECT
 public:
  explicit WebSocketServer(QObject* parent = nullptr);

  static WebSocketServer* getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
      instance = new WebSocketServer();
    }
    return instance;
  }

  void start();

  quint16 getWebSocketServerPort();

  QWebSocket* getWebSocketClient(QString clientId);

 signals:
  void connectSuccess(QString clientId);
  void sendQStringData(QString clientId, QString data);
  void sendData(QString clientId, QByteArray data);

 private:
  QWebSocketServer* server;
  static WebSocketServer* instance;
  static std::mutex mutex;
  int port = 12345;
  static QMap<QString, QWebSocket*> clientMap;

 private slots:
  void onNewConnection();
  void writeData(QString clientId, QByteArray data);
  void writeQStringData(QString clientId, QString data);
};

#endif  // WEBSOCKETSERVER_H
