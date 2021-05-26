#include "dbutil.h"
#include <QDebug>
#include "common.h"

DBUtil* DBUtil::m_pInstance=NULL;
DBUtil::DBUtil()
{

}

DBUtil* DBUtil::GetInstance()
{
     if (m_pInstance == NULL )
         m_pInstance = new DBUtil();
     return m_pInstance;
}


void DBUtil::init(){
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        QString dbName=Common::workspacePath+"ishell.db";
        database.setDatabaseName(dbName);
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    QSqlQuery sql_query;
    QString create_sql = "create table connect (id int primary key,parent_id int,auth_type int,name varchar(30), host_name varchar(100),port int,user_name varchar(100),password varchar(100),public_key_path varchar(100),private_key_path varchar(100),passPhrase varchar(100))";
    sql_query.prepare(create_sql);
    if(!sql_query.exec())
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
        QSqlQuery sql_query;
        QString sql="insert into connect (id,parent_id,name) values(?,?,?)";
        sql_query.prepare(sql);
        sql_query.addBindValue(1);
        sql_query.addBindValue(0);
        sql_query.addBindValue("默认组 ");
        sql_query.exec();
    }
}
