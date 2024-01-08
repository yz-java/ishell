#include "fileinfo.h"

#include <QDateTime>

FileInfo_S parseBySftpData(QString data) {
  FileInfo_S info;
  QList<QString> array;
  for (auto d : data.split(" ")) {
    if (!d.isEmpty()) {
      array.append(d);
    }
  }
  unsigned long timeS = array[0].toULong();
  QDateTime time = QDateTime::fromSecsSinceEpoch(timeS);
  QString updateTime = time.toString("yyyy-MM-dd hh:mm:ss");
  info.updateTime = updateTime;
  info.permission = array[1];
  info.fileNum = array[2];
  QString type = array[1].at(0);
  if (type == "d") {
    info.fileType = 1;
  } else if (type == "-") {
    info.fileType = 2;
  } else if (type == "l") {
    info.fileType = 3;
  }
  info.userOrGroup = array[3] + "/" + array[4];
  info.fileSize = array[5];

  auto names = array.mid(9);

  info.fileName = names.join(" ");

  return info;
}
