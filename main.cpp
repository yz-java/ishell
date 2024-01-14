#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  //  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
#ifdef Q_OS_LINUX
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
#endif
  QApplication a(argc, argv);
  setbuf(stdout, NULL);
  MainWindow w;
  w.show();
  return a.exec();
}
