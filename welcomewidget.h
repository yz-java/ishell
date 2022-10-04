#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>

namespace Ui {
class welcomewidget;
}

class welcomewidget : public QWidget
{
    Q_OBJECT

public:
    explicit welcomewidget(QWidget *parent = nullptr);
    ~welcomewidget();

private:
    Ui::welcomewidget *ui;
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // WELCOMEWIDGET_H
