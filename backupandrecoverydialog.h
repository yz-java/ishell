#ifndef BACKUPANDRECOVERYDIALOG_H
#define BACKUPANDRECOVERYDIALOG_H

#include <QDialog>

namespace Ui {
class BackupAndRecoveryDialog;
}

class BackupAndRecoveryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BackupAndRecoveryDialog(QWidget *parent = nullptr);
    ~BackupAndRecoveryDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_backupButton_clicked();

    void on_recoveryButton_clicked();

    void on_appendRecoveryButton_clicked();

private:
    Ui::BackupAndRecoveryDialog *ui;
    bool backupFile(QString password);
    bool recovery(QString password);
    bool appendRecovery(QString password);
};

#endif // BACKUPANDRECOVERYDIALOG_H
