#ifndef CONNECTINFO_H
#define CONNECTINFO_H
#include <QString>

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
};

#endif // CONNECTINFO_H
