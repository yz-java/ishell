QT       += core concurrent gui websockets webchannel webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QTermWidget/QTermScreen.cpp \
    QTermWidget/QTermWidget.cpp \
    backupandrecoverydialog.cpp \
    common.cpp \
    components/confirmdialog.cpp \
    connecteditui.cpp \
    connectinfo.cpp \
    connectmanagerui.cpp \
    console.cpp \
    db/connectdao.cpp \
    db/dbutil.cpp \
    fileinfo.cpp \
    main.cpp \
    mainwindow.cpp \
    mylabel.cpp \
    sftpclient.cpp \
    sftpdialog.cpp \
    sshclient.cpp \
    utils/aes.cpp \
    webconsole.cpp \
    websocketserver.cpp \
    welcome.cpp

HEADERS += \
    QTermWidget/QTermScreen.h \
    QTermWidget/QTermWidget.h \
    backupandrecoverydialog.h \
    common.h \
    components/confirmdialog.h \
    connecteditui.h \
    connectinfo.h \
    connectmanagerui.h \
    console.h \
    db/connectdao.h \
    db/dbutil.h \
    fileinfo.h \
    mainwindow.h \
    mylabel.h \
    sftpclient.h \
    sftpdialog.h \
    sshclient.h \
    utils/aes.h \
    webconsole.h \
    websocketserver.h \
    welcome.h

FORMS += \
    backupandrecoverydialog.ui \
    connecteditui.ui \
    connectmanagerui.ui \
    console.ui \
    mainwindow.ui \
    sftpdialog.ui \
    webconsole.ui \
    welcome.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QTQUICK_COMPILER_SKIPPED_RESOURCES += html.qrc

RESOURCES += \
    html.qrc \
    icon.qrc

unix:!macx: LIBS += -L$$PWD/Libs/ssh2/lib/  -lcrypto -lssl -lssh2

INCLUDEPATH += $$PWD/Libs/openssl/include $$PWD/Libs/ssh2/include
DEPENDPATH += $$PWD/Libs/openssl/include $$PWD/Libs/ssh2/include

