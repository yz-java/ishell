#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setbuf(stdout,NULL);
    MainWindow w;
    w.show();
    return a.exec();
}
