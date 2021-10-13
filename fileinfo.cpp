#include "fileinfo.h"
#include <QDateTime>

FileInfo_S parseBySftpData(QString data){
    FileInfo_S info;
    QList<QString> array;
    for(auto d:data.split(" ")){
        if(!d.isEmpty()){
            array.append(d);
        }
    }
    info.permission=array[0];
    info.fileNum=array[1];
    QString type=array[0].at(0);
    if(type=="d"){
        info.fileType=1;
    }else if(type=="-"){
        info.fileType=2;
    }else if(type=="l"){
        info.fileType=3;
    }
    info.userOrGroup=array[2]+"/"+array[3];
    info.fileSize=array[4];
    info.fileName=array[8];
    unsigned long timeS=array[9].toULong();
    QDateTime time = QDateTime::fromSecsSinceEpoch(timeS);
    QString updateTime = time.toString("yyyy-MM-dd hh:mm:ss");
    info.updateTime=updateTime;
    return info;
}
