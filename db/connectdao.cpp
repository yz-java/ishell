#include "connectdao.h"
#include "dbutil.h"

ConnectDao* ConnectDao::instance=NULL;

ConnectDao::ConnectDao()
{

}

ConnectDao* ConnectDao::GetInstance()
{
     if (instance == NULL )
         instance = new ConnectDao();
     return instance;
}

QList<ConnectInfo> ConnectDao::getConnectInfos(){
    QList<ConnectInfo> infos;
    QSqlQuery sqlQuery;
    QString select_sql = "select * from connect";
    if(!sqlQuery.exec(select_sql))
    {
        qDebug()<<sqlQuery.lastError();
    }
    else
    {
        while(sqlQuery.next())
        {
            ConnectInfo connectInfo=toConnectInfo(&sqlQuery);
            infos.append(connectInfo);
        }
    }
    return infos;
}

QList<ConnectInfo> ConnectDao::getConnectInfos(int parentId){
    QList<ConnectInfo> infos;
    QSqlQuery sqlQuery;
    QString select_sql = "select * from connect where parent_id="+QString::number(parentId);
    if(!sqlQuery.exec(select_sql))
    {
        qDebug()<<sqlQuery.lastError();
    }
    else
    {
        while(sqlQuery.next())
        {
            ConnectInfo connectInfo=toConnectInfo(&sqlQuery);
            infos.append(connectInfo);
        }
    }
    return infos;
}

ConnectInfo ConnectDao::getLastConnectInfo(){
    ConnectInfo connectInfo;
    QString sql="select * from connect order by id desc limit 0,1";
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(sql))
    {
        while(sqlQuery.next())
        {
            connectInfo=toConnectInfo(&sqlQuery);
        }
    }
    return connectInfo;
}

int ConnectDao::addConnectInfo(QString name){
    ConnectInfo info= getLastConnectInfo();
    int id=info.id+1;
    QSqlQuery sql_query;
    QString sql="insert into connect (id,parent_id,name) values(?,?,?)";
    sql_query.prepare(sql);
    sql_query.addBindValue(id);
    sql_query.addBindValue(0);
    sql_query.addBindValue(name);
    if(sql_query.exec())
    {
        return id;
    }
    qDebug()<<sql_query.lastError();
    return 0;
}

bool ConnectDao::addConnectInfo(ConnectInfo* info){
    ConnectInfo ci= getLastConnectInfo();
    int id=ci.id+1;
    QSqlQuery sql_query;
    QString sql="insert into connect values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    sql_query.prepare(sql);
    sql_query.addBindValue(id);
    sql_query.addBindValue(info->parentId);
    sql_query.addBindValue(info->authType);
    sql_query.addBindValue(info->name);
    sql_query.addBindValue(info->hostName);
    sql_query.addBindValue(info->port);
    sql_query.addBindValue(info->username);
    sql_query.addBindValue(info->password);
    sql_query.addBindValue(info->publicKeyPath);
    sql_query.addBindValue(info->privateKeyPath);
    sql_query.addBindValue(info->passPhrase);
    sql_query.addBindValue(info->vncUserName);
    sql_query.addBindValue(info->vncPassword);
    sql_query.addBindValue(info->vncPort);
    sql_query.addBindValue(info->rdpUserName);
    sql_query.addBindValue(info->rdpPassword);
    sql_query.addBindValue(info->rdpPort);
    if(sql_query.exec())
    {
        info->id=id;
        return true;
    }
    qDebug()<<sql_query.lastError();
    return false;
}

int ConnectDao::addConnectInfo(QString name,int parentId,QString hostName,QString userName,int port,QString password){
    ConnectInfo info= getLastConnectInfo();
    int id=info.id+1;
    QSqlQuery sql_query;
    QString sql="insert into connect (id,parent_id,name,host_name,user_name,port,password) values(?,?,?,?,?,?,?)";
    sql_query.prepare(sql);
    sql_query.addBindValue(id);
    sql_query.addBindValue(parentId);
    sql_query.addBindValue(name);
    sql_query.addBindValue(hostName);
    sql_query.addBindValue(userName);
    sql_query.addBindValue(port);
    sql_query.addBindValue(password);
    if(sql_query.exec())
    {
        return id;
    }
    qDebug()<<sql_query.lastError();
    return 0;
}

bool ConnectDao::updateConnectName(int id,QString name){
    QSqlQuery sql_query;
    QString sql="update connect set name=? where id=?";
    sql_query.prepare(sql);
    sql_query.addBindValue(name);
    sql_query.addBindValue(id);
    if(sql_query.exec())
    {
        return true;
    }
    qDebug()<<sql_query.lastError();
    return false;
}

bool ConnectDao::updateConnectById(ConnectInfo info){
    QSqlQuery sql_query;
    QString sql="update connect "
                "set parent_id=?,"
                "auth_type=?,"
                "name=?,"
                "host_name=?,"
                "port=?,"
                "user_name=?,"
                "password=?,"
                "public_key_path=?,"
                "private_key_path=?,"
                "passPhrase=?,"
                "vnc_username=?,"
                "vnc_password=?,"
                "vnc_port=?,"
                "rdp_username=?,"
                "rdp_password=?,"
                "rdp_port=?"
                " where id=?";
    sql_query.prepare(sql);
    sql_query.addBindValue(info.parentId);
    sql_query.addBindValue(info.authType);
    sql_query.addBindValue(info.name);
    sql_query.addBindValue(info.hostName);
    sql_query.addBindValue(info.port);
    sql_query.addBindValue(info.username);
    sql_query.addBindValue(info.password);
    sql_query.addBindValue(info.publicKeyPath);
    sql_query.addBindValue(info.privateKeyPath);
    sql_query.addBindValue(info.passPhrase);
    sql_query.addBindValue(info.vncUserName);
    sql_query.addBindValue(info.vncPassword);
    sql_query.addBindValue(info.vncPort);
    sql_query.addBindValue(info.rdpUserName);
    sql_query.addBindValue(info.rdpPassword);
    sql_query.addBindValue(info.rdpPort);
    sql_query.addBindValue(info.id);
    if(sql_query.exec())
    {
        return true;
    }
    qDebug()<<sql_query.lastError();
    return false;
}

ConnectInfo ConnectDao::getConnectInfo(int id){
    ConnectInfo connectInfo;
    QString sql="select * from connect where id="+QString::number(id);
    QSqlQuery sqlQuery;
    if(sqlQuery.exec(sql))
    {
        while(sqlQuery.next())
        {
            connectInfo=toConnectInfo(&sqlQuery);
        }
    }else{
        qDebug()<<sqlQuery.lastError();
    }
    return connectInfo;
}

bool ConnectDao::deleteById(int id){
    QSqlQuery sql_query;
    QString sql="delete from connect where id=?";
    sql_query.prepare(sql);
    sql_query.addBindValue(id);
    if(sql_query.exec())
    {
        return true;
    }
    qDebug()<<sql_query.lastError();
    return false;
}

bool ConnectDao::deleteByParentId(int parentId){
    QSqlQuery sql_query;
    QString sql="delete from connect where parent_id=?";
    sql_query.prepare(sql);
    sql_query.addBindValue(parentId);
    if(sql_query.exec())
    {
        return true;
    }
    qDebug()<<sql_query.lastError();
    return false;
}

ConnectInfo ConnectDao::toConnectInfo(QSqlQuery* sqlQuery){
    ConnectInfo connectInfo;
    int id = sqlQuery->value(0).toInt();
    connectInfo.id=id;
    int parent_id = sqlQuery->value(1).toInt();
    connectInfo.parentId=parent_id;
    int auth_type = sqlQuery->value(2).toInt();
    connectInfo.authType=auth_type;
    QString name = sqlQuery->value(3).toString();
    connectInfo.name=name;
    QString host_name = sqlQuery->value(4).toString();
    connectInfo.hostName=host_name;
    int port = sqlQuery->value(5).toInt();
    connectInfo.port=port;
    QString user_name = sqlQuery->value(6).toString();
    connectInfo.username=user_name;
    QString password = sqlQuery->value(7).toString();
    connectInfo.password=password;
    QString public_key_path = sqlQuery->value(8).toString();
    connectInfo.publicKeyPath=public_key_path;
    QString private_key_path = sqlQuery->value(9).toString();
    connectInfo.privateKeyPath=private_key_path;
    QString passPhrase = sqlQuery->value(10).toString();
    connectInfo.passPhrase=passPhrase;
    QString vncUserName = sqlQuery->value(11).toString();
    connectInfo.vncUserName=vncUserName;
    QString vncPassword = sqlQuery->value(12).toString();
    connectInfo.vncPassword=vncPassword;
    int vncPort = sqlQuery->value(13).toInt();
    connectInfo.vncPort=vncPort;

    QString rdpUserName = sqlQuery->value(14).toString();
    connectInfo.rdpUserName=rdpUserName;
    QString rdpPassword = sqlQuery->value(15).toString();
    connectInfo.rdpPassword=rdpPassword;
    int rdpPort = sqlQuery->value(16).toInt();
    connectInfo.rdpPort=rdpPort;
    return connectInfo;
}

bool ConnectDao::deleteAll(){
    QSqlQuery sql_query;
    QString sql="delete from connect";
    sql_query.prepare(sql);
    if(sql_query.exec())
    {
        return true;
    }
    qDebug()<<sql_query.lastError();
    return false;
}


