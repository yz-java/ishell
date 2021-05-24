#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
class WebSocketServer:public QWidget
{
    Q_OBJECT
public:
    WebSocketServer(QWidget *parent);

    static WebSocketServer* instance;

    void run();

    static QWebSocket* getClient(QString clientId);

    static void deleteClient(QString clientId);

    void sendMsg(QString clientId,QString msg);

private:
    QWebSocketServer* webSocketServer;
    static QMap<QString,QWebSocket*> clientMap;

public Q_SLOTS:
    void newConnection();

signals:
    void sendMessage(const QString& msg);
};

#endif // WEBSOCKETSERVER_H
