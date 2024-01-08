#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "common.h"
#include "connectmanagerui.h"
#include "welcomewidget.h"
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void resizeEvent(QResizeEvent *);
  //组合按键处理
  void keyPressEvent(QKeyEvent *e);

 signals:
  void alertErrorMessageBox(QString msg);

 private slots:
  void on_tabWidget_currentChanged(int index);

  void on_tabWidget_tabCloseRequested(int index);

  void openSSHConnect(ConnectInfo connectInfo);

  void openVNCConnect(ConnectInfo connectInfo);

  void openRDPConnect(ConnectInfo connectInfo);

  void openSFTPConnect(ConnectInfo connectInfo);

 private:
  Ui::MainWindow *ui;
  ConnectManagerUI *connectManagerUI;
  int currentIndex;
  void initUI();
};
#endif  // MAINWINDOW_H

extern MainWindow *mainwindow;
