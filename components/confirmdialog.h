#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H
#include <QDialog>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class ConfirmDialog:public QDialog
{
    Q_OBJECT
public:

    ConfirmDialog(QWidget *parent = nullptr,QString title="");

    ConfirmDialog(QWidget *parent,QString title,QString okButtonName);

    ConfirmDialog(QWidget *parent,QString title,QString okButtonName,QString cancelButtonName);


signals:
    void successEdit(QString input);
};

#endif // CONFIRMDIALOG_H
