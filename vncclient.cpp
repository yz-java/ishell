#include "vncclient.h"

#include <QDebug>

static rfbBool resize(rfbClient *client) {
  int width = client->width, height = client->height,
      depth = client->format.bitsPerPixel;

  client->updateRect.x = client->updateRect.y = 0;
  client->updateRect.w = width;
  client->updateRect.h = height;
  VncClient *c = (VncClient *)rfbClientGetClientData(client, 0);
  //  client->width = sdl->pitch / (depth / 8);
  c->colorDepth = depth;
  c->screenSizeEvent(width, height);
  switch (depth) {
    case 8:
      client->format.depth = 8;
      client->format.bitsPerPixel = 8;
      client->format.redShift = 0;
      client->format.greenShift = 3;
      client->format.blueShift = 6;
      client->format.redMax = 7;
      client->format.greenMax = 7;
      client->format.blueMax = 3;
      break;
    case 16:
      client->format.depth = 16;
      client->format.bitsPerPixel = 16;
      client->format.redShift = 11;
      client->format.greenShift = 5;
      client->format.blueShift = 0;
      client->format.redMax = 0x1f;
      client->format.greenMax = 0x3f;
      client->format.blueMax = 0x1f;
      break;
    case 32:
    default:
      client->format.depth = 24;
      client->format.bitsPerPixel = 32;
      client->format.redShift = 0;
      client->format.greenShift = 8;
      client->format.blueShift = 16;
      client->format.redMax = 0xff;
      client->format.greenMax = 0xff;
      client->format.blueMax = 0xff;
  }

  if (c->frameBuffer) {
    delete[] c->frameBuffer;
  }
  const int size = width * height * (depth / 8);
  c->frameBuffer = new uint8_t[size];
  memset(c->frameBuffer, '\0', size);
  client->frameBuffer = c->frameBuffer;

  SetFormatAndEncodings(client);
  return TRUE;
}

static void update(rfbClient *cl, int x, int y, int w, int h) {
  VncClient *c = (VncClient *)rfbClientGetClientData(cl, 0);
  Q_ASSERT(c);

  //  const int width = cl->width, height = cl->height;
  //  QImage img;
  //  switch (c->colorDepth) {
  //    case 8:
  //      img = QImage(cl->frameBuffer, width, height, QImage::Format_Indexed8);
  //      //      img.setColorTable(m_colorTable);
  //      break;
  //    case 16:
  //      img = QImage(cl->frameBuffer, width, height, QImage::Format_RGB16);
  //      break;
  //    case 32:
  //      img = QImage(cl->frameBuffer, width, height, QImage::Format_RGB32);
  //      break;
  //  }

  //  if (img.isNull()) {
  //    qDebug() << "image not loaded";
  //  }
  //  c->updateImageEvent(img, x, y, w, h);
  c->frameUpdateEvent(cl->frameBuffer, x, y, w, h);
}

static void kbd_leds(rfbClient *cl, int value, int pad) {
  /* note: pad is for future expansion 0=unused */
  fprintf(stderr, "Led State= 0x%02X\n", value);
  fflush(stderr);
}

VncClient::VncClient(QString hostName, int port, QString password) : QThread() {
  this->hostName = hostName;
  this->port = port;
  this->password = password;
}

void VncClient::close() {
  running = false;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if (cl->sock) {
    ::close(cl->sock);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if (frameBuffer) {
    delete[] frameBuffer;
    frameBuffer = NULL;
  }
}

void VncClient::sendPointerEvent(int x, int y, int buttonMask) {
  SendPointerEvent(cl, x, y, buttonMask);
}

void VncClient::sendKeyEvent(int key, bool upOrDown) {
  SendKeyEvent(cl, key, upOrDown);
}

static void text_chat(rfbClient *cl, int value, char *text) {
  switch (value) {
    case (int)rfbTextChatOpen:
      fprintf(stderr, "TextChat: We should open a textchat window!\n");
      TextChatOpen(cl);
      break;
    case (int)rfbTextChatClose:
      fprintf(stderr, "TextChat: We should close our window!\n");
      break;
    case (int)rfbTextChatFinished:
      fprintf(stderr, "TextChat: We should close our window!\n");
      break;
    default:
      fprintf(stderr, "TextChat: Received \"%s\"\n", text);
      break;
  }
  fflush(stderr);
}

static void got_selection_latin1(rfbClient *cl, const char *text, int len) {
  rfbClientLog("received latin1 clipboard text '%s'\n", text);
}

static char *ReadPassword(rfbClient *cl) {
  VncClient *c = (VncClient *)rfbClientGetClientData(cl, 0);
  char *password = (char *)malloc(c->password.size());
  memset(password, 0, c->password.size());
  memcpy(password, c->password.toLocal8Bit(), c->password.size());
  return password;
}

static void cleanup(rfbClient *cl) {
  if (cl) rfbClientCleanup(cl);
}

void VncClient::run() {
  /* 16-bit: cl=rfbGetClient(5,3,2); */
  cl = rfbGetClient(8, 3, 4);
  cl->MallocFrameBuffer = resize;
  cl->canHandleNewFBSize = TRUE;
  cl->GotFrameBufferUpdate = update;
  cl->HandleKeyboardLedState = kbd_leds;
  cl->HandleTextChat = text_chat;
  cl->GotXCutText = got_selection_latin1;
  rfbClientSetClientData(cl, 0, this);
  cl->GetPassword = ReadPassword;
  //    cl->listenPort = port;
  //    cl->listen6Port = port;
  cl->serverPort = port;
  char *host_name = (char *)malloc(hostName.size());
  memset(host_name, '\0', hostName.size());
  memcpy(host_name, hostName.toLocal8Bit().data(), hostName.size());
  cl->serverHost = host_name;

  if (!rfbInitClient(cl, 0, NULL)) {
    return;
  }
  std::thread t([=]() {
    int i = 0;
    while (running) {
      i = WaitForMessage(cl, 500);
      if (i < 0) {
        cleanup(cl);
        break;
      }
      if (i)
        if (!HandleRFBServerMessage(cl)) {
          cleanup(cl);
          break;
        }
    }
  });
  t.detach();
  exec();
}
