QT       += core gui websockets webchannel webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common.cpp \
    components/confirmdialog.cpp \
    connecteditui.cpp \
    connectinfo.cpp \
    connectmanagerui.cpp \
    console.cpp \
    db/connectdao.cpp \
    db/dbutil.cpp \
    main.cpp \
    mainwindow.cpp \
    mylabel.cpp \
    sshclient.cpp \
    webconsole.cpp \
    welcome.cpp

HEADERS += \
    common.h \
    components/confirmdialog.h \
    connecteditui.h \
    connectinfo.h \
    connectmanagerui.h \
    console.h \
    db/connectdao.h \
    db/dbutil.h \
    mainwindow.h \
    mylabel.h \
    sshclient.h \
    webconsole.h \
    welcome.h

FORMS += \
    connecteditui.ui \
    connectmanagerui.ui \
    console.ui \
    mainwindow.ui \
    webconsole.ui \
    welcome.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    html.qrc \
    icon.qrc

win32: LIBS += -L$$PWD/Libs/ssh2/lib/ -L$$PWD/Libs/openssl/lib/ -lws2_32  -llibcrypto -llibssl -llibssh2

INCLUDEPATH += $$PWD/Libs/openssl/include $$PWD/Libs/ssh2/include
DEPENDPATH += $$PWD/Libs/openssl/include $$PWD/Libs/ssh2/include

