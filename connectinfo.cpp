#include "connectinfo.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
ConnectInfo::ConnectInfo()
{

}

ConnectInfo ConnectInfo::jsonStrToConnectInfo(QString jsonStr){
    ConnectInfo info;
    QJsonDocument document=QJsonDocument::fromJson(jsonStr.toUtf8());
    if(!document.isNull()){
        return info;
    }
    if(document.isObject()){
        QJsonObject obj= document.object();
        info=jsonObjToConnectInfo(obj);
    }
    return info;
}

ConnectInfo ConnectInfo::jsonObjToConnectInfo(QJsonObject obj){
    ConnectInfo info;
    int id = obj.take("id").toInt();
    info.id=id;
    int authType = obj.take("authType").toInt();
    info.authType=authType;
    QString name = obj.take("name").toString();
    info.name=name;
    QString hostName = obj.take("hostName").toString();
    info.hostName=hostName;
    QString username = obj.take("username").toString();
    info.username=username;
    QString password = obj.take("password").toString();
    info.password=password;
    QString publicKeyPath = obj.take("publicKeyPath").toString();
    info.publicKeyPath=publicKeyPath;
    QString privateKeyPath = obj.take("privateKeyPath").toString();
    info.privateKeyPath=privateKeyPath;
    QString passPhrase = obj.take("passPhrase").toString();
    info.passPhrase=passPhrase;
    int port = obj.take("port").toInt();
    info.port=port==0?22:port;
    int parentId = obj.take("parentId").toInt();
    info.parentId=parentId;

    QString vncUserName = obj.take("vncUserName").toString();
    QString vncPassword = obj.take("vncPassword").toString();
    int vncPort = obj.take("vncPort").toInt();
    info.vncUserName=vncUserName;
    info.vncPassword=vncPassword;
    info.vncPort=vncPort==0?5900:vncPort;

    QString rdpUserName = obj.take("rdpUserName").toString();
    QString rdpPassword = obj.take("rdpPassword").toString();
    int rdpPort = obj.take("vncPort").toInt();
    info.rdpUserName=rdpUserName;
    info.rdpPassword=rdpPassword;
    info.rdpPort=rdpPort==0?3389:rdpPort;
    return info;
}
