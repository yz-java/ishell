#include "confirmdialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ConfirmDialog::ConfirmDialog(QWidget *parent, QString title)
    : ConfirmDialog(parent, title, "修改", "取消") {}

ConfirmDialog::ConfirmDialog(QWidget *parent, QString title,
                             QString okButtonName)
    : ConfirmDialog(parent, title, okButtonName, "取消") {}

ConfirmDialog::ConfirmDialog(QWidget *parent, QString title,
                             QString okButtonName, QString cancelButtonName)
    : QDialog(parent) {
  if (!title.isEmpty()) {
    setWindowTitle(title);
  }
  setModal(true);
#ifdef Q_OS_MAC
  setWindowFlags(windowFlags() &
                 ~Qt::WindowMaximizeButtonHint);  // 禁止最大化按钮
  setFixedSize(200, 100);
#endif

  QVBoxLayout *rootlayout = new QVBoxLayout();
  inputEdit = new QLineEdit(this);
  rootlayout->addWidget(inputEdit);
  QHBoxLayout *hlayout = new QHBoxLayout();
  QPushButton *cancelButton = new QPushButton(cancelButtonName);
  cancelButton->setAutoDefault(false);
  connect(cancelButton, &QPushButton::clicked, [=]() { this->close(); });
  okButton = new QPushButton(okButtonName, this);
  okButton->setAutoDefault(true);
  connect(okButton, &QPushButton::clicked, [=]() {
    emit confirmEditEvent(inputEdit->text());
    this->close();
  });
  hlayout->addWidget(cancelButton);
  hlayout->addWidget(okButton);
  rootlayout->addLayout(hlayout);

  this->setLayout(rootlayout);
}

void ConfirmDialog::setTitleText(QString titleText) {
  setWindowTitle(titleText);
}

void ConfirmDialog::setEditText(QString editText) {
  inputEdit->setText(editText);
}

void ConfirmDialog::setOkButtonName(QString buttonName) {
  okButton->setText(buttonName);
}
