#ifndef FILEINFO_H
#define FILEINFO_H
#include <QObjectUserData>
#include <QString>
#include <QStringList>

struct FileInfo_S {

  QString filePath;

  QString fileName;

  QString fileSize;
  // 1=文件夹 2=文件 3=符号链接 4=FIFO管道 5=套接字 6=字符设备 7=块设备
  int fileType;

  QString permission;

  QString userOrGroup;

  QString fileNum;

  QString updateTime;
};
Q_DECLARE_METATYPE(FileInfo_S)

FileInfo_S parseBySftpData(QString data);

#endif // FILEINFO_H
