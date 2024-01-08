#ifndef VNCPROXYCLIENT_H
#define VNCPROXYCLIENT_H

#include <QObject>
#include <QTcpSocket>
class VncProxyClient : public QObject {
  Q_OBJECT
 public:
  explicit VncProxyClient(QString clientId, QString host, int port);
  ~VncProxyClient();
 public slots:
  void connectToHost();
 private slots:

  void connected();
  void readData();
  void disconnectFromHost();

 private:
  QTcpSocket* socket;
  QString host;
  QString clientId;
  int port;
 signals:
  void disconnected();
  void writeData(QByteArray data);
};

#endif  // VNCPROXYCLIENT_H
