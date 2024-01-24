#ifndef VNCCLIENT_H
#define VNCCLIENT_H
#include <rfb/rfbclient.h>

#include <QImage>
#include <QThread>
class VncClient : public QThread {
  Q_OBJECT
 public:
  explicit VncClient(QString hostName, int port, QString password);

  QString hostName;
  int port;
  QString password;

  uint8_t* frameBuffer = NULL;

  int colorDepth;

  void close();

  void sendPointerEvent(int x, int y, int buttonMask);

  void sendKeyEvent(int key, bool upOrDown);

 private:
  rfbClient* cl;

  bool running = true;

  void run() override;

 signals:
  void updateImageEvent(QImage image, int x, int y, int w, int h);
  void frameUpdateEvent(uint8_t* frameBuffer, int x, int y, int w, int h);
  void screenSizeEvent(int w, int h);
};

#endif  // VNCCLIENT_H
