#include "vncviewerwidget.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
VncViewerWidget::VncViewerWidget(QWidget *parent, ConnectInfo info)
    : QWidget(parent) {
  this->info = info;
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);
  client = new VncClient(info.hostName, info.vncPort, info.vncPassword);
  client->start();
  connect(client, &VncClient::screenSizeEvent, this, [=](int w, int h) {
    ;
    setFixedSize(w, h);
  });
  connect(client, &VncClient::updateImageEvent, this,
          [=](QImage img, int x, int y, int w, int h) {
            this->img = img;
            repaint(x, y, w, h);
          });
}

void VncViewerWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.drawImage(QRect(0, 0, width(), height()), img);
}

void VncViewerWidget::mouseMoveEvent(QMouseEvent *ev) {
  int x = ev->x();
  int y = ev->y();
  client->sendPointerEvent(x, y, buttonMask);
}

void VncViewerWidget::mousePressEvent(QMouseEvent *e) {
  if (e->button() & Qt::LeftButton) buttonMask |= 0x01;
  if (e->button() & Qt::MidButton) buttonMask |= 0x02;
  if (e->button() & Qt::RightButton) buttonMask |= 0x04;
  int x = e->x();
  int y = e->y();
  client->sendPointerEvent(x, y, buttonMask);
  //  buttonMask &= ~(rfbButton4Mask | rfbButton5Mask);
}

void VncViewerWidget::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() & Qt::LeftButton) buttonMask &= 0xfe;
  if (e->button() & Qt::MidButton) buttonMask &= 0xfd;
  if (e->button() & Qt::RightButton) buttonMask &= 0xfb;
  int x = e->x();
  int y = e->y();
  client->sendPointerEvent(x, y, buttonMask);
  //  buttonMask &= ~(rfbButton4Mask | rfbButton5Mask);
}

void VncViewerWidget::wheelEvent(QWheelEvent *event) {
  int steps;
  int x = event->x();
  int y = event->y();
  int delta = event->delta();
  for (steps = 0; steps < 10; ++steps) {
    if (delta > 0) {
      client->sendPointerEvent(x, y, rfbWheelUpMask);
      client->sendPointerEvent(x, y, 0);
    } else {
      client->sendPointerEvent(x, y, rfbWheelDownMask);
      client->sendPointerEvent(x, y, 0);
    }
  }
}

void VncViewerWidget::keyPressEvent(QKeyEvent *event) {
  rfbKeySym k = event->nativeVirtualKey();
  client->sendKeyEvent(k, true);
}

void VncViewerWidget::keyReleaseEvent(QKeyEvent *event) {
  rfbKeySym k = event->nativeVirtualKey();
  client->sendKeyEvent(k, false);
}

bool VncViewerWidget::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::Close) {
    client->close();
    this->close();
  }

  return QWidget::eventFilter(watched, event);
}
