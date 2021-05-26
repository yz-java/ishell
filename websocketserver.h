#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
class WebSocketServer:public QObject
{
    Q_OBJECT
public:
    WebSocketServer();

    static WebSocketServer* instance;

    void run();

    static QWebSocket* getClient(QString clientId);

    static void deleteClient(QString clientId);

    void sendMsg(QString clientId,QString msg);

private:
    QWebSocketServer* webSocketServer;
    static QMap<QString,QWebSocket*> clientMap;

public slots:
    void newConnection();

signals:
    void sendMessage(const QString& msg);
    void errorMsg(const QString& msg);
};

#endif // WEBSOCKETSERVER_H
