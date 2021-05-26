#include "websocketserver.h"
#include <QMessageBox>
QMap<QString,QWebSocket*> WebSocketServer::clientMap;
WebSocketServer* WebSocketServer::instance=NULL;
WebSocketServer::WebSocketServer()
{
    instance=this;
}

void WebSocketServer::run(){
    webSocketServer=new QWebSocketServer("Server",QWebSocketServer::NonSecureMode);
    if (!webSocketServer->listen(QHostAddress::LocalHost, 12345)) {
        qDebug() << "12345端口已被占用";
        emit errorMsg("12345端口已被占用");
        return;
    }
    connect(webSocketServer,&QWebSocketServer::newConnection,this,&WebSocketServer::newConnection);
}

void WebSocketServer::newConnection(){
    QWebSocket *websocket=webSocketServer->nextPendingConnection();
    if(!websocket){
        return;
    }
    connect(websocket,&QWebSocket::textMessageReceived,this,[=](const QString& msg){
        clientMap[msg]=websocket;
        qDebug() << "新增客户端" << clientMap.count();
    },Qt::QueuedConnection);

    connect(websocket,&QWebSocket::disconnected,this,[=](){
        qDebug() << "客户端退出";
    });

}
QWebSocket* WebSocketServer::getClient(QString clientId){
    return clientMap[clientId];
}

void WebSocketServer::deleteClient(QString clientId){
    clientMap.remove(clientId);
}

void WebSocketServer::sendMsg(QString clientId,QString msg){
//    qDebug() << "客户端数量" << clientMap.count();
    clientMap[clientId]->sendTextMessage(msg);
}
