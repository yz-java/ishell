#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H
#include <QDialog>
#include <QLineEdit>
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class ConfirmDialog : public QDialog {
  Q_OBJECT
 public:
  ConfirmDialog(QWidget *parent = nullptr, QString title = "");

  ConfirmDialog(QWidget *parent, QString title, QString okButtonName);

  ConfirmDialog(QWidget *parent, QString title, QString okButtonName,
                QString cancelButtonName);

  void setTitleText(QString titleText);

  void setEditText(QString editText);

  void setOkButtonName(QString buttonName);

 private:
  QLineEdit *inputEdit;
  QPushButton *okButton;

 signals:
  void confirmEditEvent(QString input);
};

#endif  // CONFIRMDIALOG_H
