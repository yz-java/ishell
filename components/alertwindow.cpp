#include "alertwindow.h"

#include <QResizeEvent>
#include <QTimer>

#include "ui_alertwindow.h"
AlertWindow::AlertWindow(QWidget *parent, bool hideOnClose,
                         int autoCloseSeconds)
    : QWidget(parent), ui(new Ui::AlertWindow) {
  ui->setupUi(this);
  this->hideOnClose = hideOnClose;
  this->autoCloseSeconds = autoCloseSeconds;
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_DeleteOnClose);
  if (parent) {
    resize(parent->size());
  }
  if (autoCloseSeconds > 0) {
    cancelButtonHide();
    confirmButtonHide();
    QTimer::singleShot(autoCloseSeconds * 1000, [=]() { this->close(); });
  }
}

AlertWindow::~AlertWindow() { delete ui; }

void AlertWindow::setTitleText(QString title) {
  ui->titleLabel->setText(title);
}

void AlertWindow::setContentText(QString contentText) {
  ui->contentLabel->setText(contentText);
}

void AlertWindow::setConfirmButtonText(QString buttonText) {
  ui->confirmPushButton->setText(buttonText);
}

void AlertWindow::setCancelButtonText(QString buttonText) {
  ui->cancelPushButton->setText(buttonText);
}

void AlertWindow::confirmButtonHide() { ui->confirmPushButton->hide(); }

void AlertWindow::cancelButtonHide() { ui->cancelPushButton->hide(); }

void AlertWindow::confirmButtonShow() { ui->confirmPushButton->show(); }

void AlertWindow::cancelButtonShow() { ui->cancelPushButton->show(); }

bool AlertWindow::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::Resize) {
    QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
    resize(resizeEvent->size());
    return false;
  }
  return true;
}

void AlertWindow::on_confirmPushButton_clicked() {
  if (hideOnClose) {
    this->close();
  } else {
    this->hide();
  }
  emit confirmEvent();
}

void AlertWindow::on_cancelPushButton_clicked() {
  if (hideOnClose) {
    this->close();
  } else {
    this->hide();
  }
  emit cancelEvent();
}
