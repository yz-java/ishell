#include "vncviewerwidget.h"

#ifdef Q_OS_LINUX
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPainter>
#include <QWheelEvent>

static GLfloat vertices[] = {
    -1.0f, 1.0f,  0.0f,  // Top-left
    -1.0f, -1.0f, 0.0f,  // Bottom-left
    1.0f,  1.0f,  0.0f,  // Top-right
    1.0f,  -1.0f, 0.0f   // Bottom-right
};

static GLfloat texCoords[] = {
    0.0f, 0.0f,  // Bottom-left
    0.0f, 1.0f,  // Top-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f   // Top-right
};

VncViewerWidget::VncViewerWidget(QWidget *parent, ConnectInfo info)
    : QOpenGLWidget(parent) {
  this->info = info;
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);
  client = new VncClient(info.hostName, info.vncPort, info.vncPassword);
  client->start();
  connect(client, &VncClient::screenSizeEvent, this, [=](int w, int h) {
    this->width = w;
    this->height = h;
    this->frameBuffer = new uint8_t[w * h * 4];
    setFixedSize(w, h);
  });
  connect(client, &VncClient::updateImageEvent, this,
          [=](QImage img, int x, int y, int w, int h) {
            this->img = img;
            repaint(x, y, w, h);
          });
  connect(client, &VncClient::frameUpdateEvent, this,
          [=](uint8_t *framebuffer, int x, int y, int w, int h) {
            memcpy(this->frameBuffer, framebuffer,
                   this->width * this->height * 4);
            repaint(x, y, w, h);
          });
}

void VncViewerWidget::initializeGL() {
  QString glType;
  QString glVersion;
  QString glProfile;

  // 获取版本信息
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));

  printf("type is %s\n", glType.toStdString().data());

  printf("version is %s\n", glVersion.toStdString().data());

  // program加载shader（顶点和片元）脚本

  // 顶点shader
  program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl");

  // 片元（像素）
  program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl");

  // 编译shader
  program.link();

  program.bind();

  int position = program.attributeLocation("position");
  int textureIndex = program.attributeLocation("textureIndex");
  // 顶点
  glVertexAttribPointer(position, 3, GL_FLOAT, 0, 0, vertices);
  glEnableVertexAttribArray(position);

  //纹理坐标
  glVertexAttribPointer(textureIndex, 2, GL_FLOAT, 0, 0, texCoords);
  glEnableVertexAttribArray(textureIndex);

  //创建纹理
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void VncViewerWidget::paintGL() {
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width, height);
  int inputTexture = program.uniformLocation("inputTexture");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, frameBuffer);

  glUniform1i(inputTexture, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  //  glFlush();
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
    glDeleteTextures(1, &texture);
    this->close();
  }

  return QWidget::eventFilter(watched, event);
}
