#include "welcomewidget.h"
#include "ui_welcomewidget.h"
#include <QPainter>
welcomewidget::welcomewidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::welcomewidget)
{

    ui->setupUi(this);
}

welcomewidget::~welcomewidget()
{
    delete ui;
}

void welcomewidget::resizeEvent(QResizeEvent *event)
{

}

void welcomewidget::paintEvent(QPaintEvent *event)
{
//    QPainter p(this);
//    p.setPen(Qt::NoPen);
//    /* 设置红色 */
//    p.setBrush(Qt::red);
//    p.drawRect(rect());
}
