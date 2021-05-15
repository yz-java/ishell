#ifndef DBUTIL_H
#define DBUTIL_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class DBUtil
{
private:
    static DBUtil * m_pInstance;

public:
    DBUtil();
    static DBUtil* GetInstance();
    QSqlDatabase database;
    void init();
};

#endif // DBUTIL_H
