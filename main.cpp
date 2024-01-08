#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  //  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
  QApplication a(argc, argv);
  setbuf(stdout, NULL);
  MainWindow w;
  w.show();
  return a.exec();
}
