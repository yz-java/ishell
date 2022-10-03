#ifndef SDLVNCVIEWER_H
#define SDLVNCVIEWER_H
#include <stdlib.h>

typedef void (*ErrorCallBack)(char* msg) ;

void openVNCViewer(const char*host,const char* vncName,const char* vncPassword,int port,ErrorCallBack errorCallback);

#endif // SDLVNCVIEWER_H
