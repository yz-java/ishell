#include "confirmdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

ConfirmDialog::ConfirmDialog(QWidget *parent,QString title):QDialog(parent)
{
    if(!title.isEmpty()){
        setWindowTitle(title);
    }

#ifdef Q_OS_MAC
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(200,100);
#endif

    QVBoxLayout* rootlayout=new QVBoxLayout();
    QLineEdit* inputEdit=new QLineEdit();
    rootlayout->addWidget(inputEdit);
    QHBoxLayout* hlayout=new QHBoxLayout();
    QPushButton* cancelButton=new QPushButton("取消");
    cancelButton->setAutoDefault(false);
    connect(cancelButton,&QPushButton::clicked,[=](){
        this->close();
    });
    QPushButton* okButton=new QPushButton("修改");
    okButton->setAutoDefault(true);
    connect(okButton,&QPushButton::clicked,[=](){
        emit successEdit(inputEdit->text());
        this->close();
    });
    hlayout->addWidget(cancelButton);
    hlayout->addWidget(okButton);
    rootlayout->addLayout(hlayout);

    this->setLayout(rootlayout);
}
