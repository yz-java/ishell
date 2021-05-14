#ifndef DBUTIL_H
#define DBUTIL_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
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
