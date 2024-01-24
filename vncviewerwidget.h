#ifndef VNCVIEWERWIDGET_H
#define VNCVIEWERWIDGET_H

#include <QGLShaderProgram>
#include <QOpenGLWidget>

#include "connectinfo.h"
#include "vncclient.h"
class VncViewerWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit VncViewerWidget(QWidget* parent, ConnectInfo info);

 private:
  ConnectInfo info;
  VncClient* client;
  QImage img;
  int buttonMask = 0;

  QGLShaderProgram program;  // shader程序

  uint8_t* frameBuffer;

  GLuint texture;

  int width;

  int height;

  void initializeGL();

  void paintGL();
  //鼠标移动事件
  void mouseMoveEvent(QMouseEvent* ev);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

  void wheelEvent(QWheelEvent* event);

  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

  bool eventFilter(QObject* watched, QEvent* event);

 signals:
};

#endif  // VNCVIEWERWIDGET_H
