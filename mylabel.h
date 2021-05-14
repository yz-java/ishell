#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>
#include <QMouseEvent>

class MyLabel:public QLabel
{
    Q_OBJECT
public:
    MyLabel(const QString & text="",QWidget * parent = 0);
    void mousePressEvent(QMouseEvent *event);


signals:
    void click();

};

#endif // MYLABEL_H
