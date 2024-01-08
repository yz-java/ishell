#ifndef ALERTWINDOW_H
#define ALERTWINDOW_H

#include <QDialog>

namespace Ui {
class AlertWindow;
}

class AlertWindow : public QWidget {
  Q_OBJECT

 public:
  explicit AlertWindow(QWidget *parent = nullptr, bool hideOnClose = false,
                       int autoCloseSeconds = 0);
  ~AlertWindow();

  void setTitleText(QString title);

  void setContentText(QString contentText);

  void setConfirmButtonText(QString buttonText);

  void setCancelButtonText(QString buttonText);

  void confirmButtonHide();

  void cancelButtonHide();

  void confirmButtonShow();

  void cancelButtonShow();

 private:
  Ui::AlertWindow *ui;

  bool hideOnClose;

  int autoCloseSeconds;

//  void resizeEvent(QResizeEvent*);
  bool eventFilter(QObject *watched, QEvent *event) override;

 signals:
  void confirmEvent();
  void cancelEvent();
 private slots:
  void on_confirmPushButton_clicked();
  void on_cancelPushButton_clicked();
};

#endif  // ALERTWINDOW_H
