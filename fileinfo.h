#ifndef FILEINFO_H
#define FILEINFO_H
#include <QString>
#include <QStringList>
#include <QObjectUserData>

struct FileInfo_S {

    QString filePath;

    QString fileName;

    QString fileSize;
    //1=文件夹 2=文件
    int fileType;

    QString permission;

    QString userOrGroup;

    QString fileNum;

};
Q_DECLARE_METATYPE(FileInfo_S)

FileInfo_S parseBySftpData(QString data);



#endif // FILEINFO_H
