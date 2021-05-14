#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H
#include <QDialog>

class ConfirmDialog:public QDialog
{
    Q_OBJECT
public:
    ConfirmDialog(QWidget *parent = nullptr,QString title="");

signals:
    void successEdit(QString input);
};

#endif // CONFIRMDIALOG_H
