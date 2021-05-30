#ifndef CONNECTDAO_H
#define CONNECTDAO_H
#include <QList>
#include "connectinfo.h"
#include <QSqlQuery>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class ConnectDao
{
public:
    ConnectDao();
    static ConnectDao* GetInstance();

    QList<ConnectInfo> getConnectInfos();

    QList<ConnectInfo> getConnectInfos(int parentId);

    ConnectInfo getLastConnectInfo();

    int addConnectInfo(QString name);

    bool addConnectInfo(ConnectInfo* info);

    int addConnectInfo(QString name,int parentId,QString hostName,QString userName,int port,QString password);

    bool updateConnectName(int id,QString name);

    bool updateConnectById(ConnectInfo info);

    ConnectInfo getConnectInfo(int id);

    ConnectInfo toConnectInfo(QSqlQuery* query);

    bool deleteById(int id);

    bool deleteByParentId(int parentId);

    bool deleteAll();

private:
    static ConnectDao* instance;
};

#endif // CONNECTDAO_H
