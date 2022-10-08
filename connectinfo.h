#ifndef CONNECTINFO_H
#define CONNECTINFO_H
#include <QString>
#include <QJsonObject>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class ConnectInfo
{
public:
    ConnectInfo();

    int id=0;

    int parentId=0;

    QString name="";

    int port = 22;

    QString hostName="";

    QString username="";

    QString password="";

    QString publicKeyPath="";

    QString privateKeyPath="";
    //私钥密码
    QString passPhrase="";
    //1=帐号密码  2=公私钥
    int authType=1;

    QString vncUserName = "";

    QString vncPassword = "";

    int vncPort = 5900;

    QString rdpUserName = "";

    QString rdpPassword = "";

    int rdpPort = 3389;

    static ConnectInfo jsonStrToConnectInfo(QString jsonStr);

    static ConnectInfo jsonObjToConnectInfo(QJsonObject obj);
};

#endif // CONNECTINFO_H
