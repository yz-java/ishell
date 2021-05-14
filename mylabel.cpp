#include "mylabel.h"
#include <QMessageBox>
MyLabel::MyLabel(const QString & text,QWidget *parent) : QLabel(parent)
{
    setText(text);
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    //Qt::LeftButton
    //Qt::RightButton
    if(event->button()== Qt::LeftButton)
    {
        emit click();
    }
}
