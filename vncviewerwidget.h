#ifndef VNCVIEWERWIDGET_H
#define VNCVIEWERWIDGET_H

#include <QWidget>

#include "connectinfo.h"
#include "vncclient.h"
class VncViewerWidget : public QWidget {
  Q_OBJECT
 public:
  explicit VncViewerWidget(QWidget* parent, ConnectInfo info);

 private:
  ConnectInfo info;
  VncClient* client;
  QImage img;
  int buttonMask = 0;
  void paintEvent(QPaintEvent* event);
  //鼠标移动事件
  void mouseMoveEvent(QMouseEvent* ev);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

  void wheelEvent(QWheelEvent* event);

  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

 signals:
};

#endif  // VNCVIEWERWIDGET_H
