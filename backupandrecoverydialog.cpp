#include "backupandrecoverydialog.h"
#include "common.h"
#include "components/confirmdialog.h"
#include "db/connectdao.h"
#include "ui_backupandrecoverydialog.h"
#include "utils/aes.h"
#include <QDebug>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

QString toJsonStr(QList<ConnectInfo> list) {

  QJsonArray array;
  for (ConnectInfo info : list) {
    QJsonObject obj;
    obj.insert("id", info.id);
    obj.insert("parentId", info.parentId);
    obj.insert("name", info.name);
    obj.insert("port", info.port);
    obj.insert("hostName", info.hostName);
    obj.insert("username", info.username);
    obj.insert("authType", info.authType);
    obj.insert("publicKeyPath", info.publicKeyPath);
    obj.insert("privateKeyPath", info.privateKeyPath);
    obj.insert("vncUserName", info.vncUserName);
    obj.insert("vncPassword", info.vncPassword);
    obj.insert("vncPort", info.vncPort);
    obj.insert("rdpUserName", info.rdpUserName);
    obj.insert("rdpPassword", info.rdpPassword);
    obj.insert("rdpPort", info.rdpPort);
    array.append(obj);
  }
  QJsonDocument document;
  document.setArray(array);
  QByteArray byteArray = document.toJson(QJsonDocument::Compact);
  QString strJson(byteArray);
  return strJson;
}

BackupAndRecoveryDialog::BackupAndRecoveryDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::BackupAndRecoveryDialog) {
  ui->setupUi(this);
  setWindowTitle("备份-恢复");
  setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
  setFixedSize(this->width(), this->height());
}

BackupAndRecoveryDialog::~BackupAndRecoveryDialog() { delete ui; }
//备份选择按钮
void BackupAndRecoveryDialog::on_pushButton_clicked() {
  QString directory = QFileDialog::getExistingDirectory(
      this, tr("选择备份位置"), nullptr, QFileDialog::DontUseNativeDialog);

  //    QFileDialog* dialog=new QFileDialog();
  //    dialog->setFileMode(QFileDialog::Directory);
  //    dialog->setWindowTitle("选择备份位置");
  //    if(!dialog->exec()){
  //        return;
  //    }
  //    QString localPath = dialog->selectedFiles().at(0);
  ui->backupPath->setText(directory);
}
//恢复选择按钮
void BackupAndRecoveryDialog::on_pushButton_2_clicked() {
  QString filePath =
      QFileDialog::getOpenFileName(this, tr("选择文件恢复"), "", tr("*"), 0,
                                   QFileDialog::DontUseNativeDialog);
  ui->recoveryFilePath->setText(filePath);
}

void BackupAndRecoveryDialog::on_backupButton_clicked() {
  QString backupPath = ui->backupPath->text();
  if (backupPath.isEmpty()) {
    QMessageBox::warning(this, "提示", "请选择备份路径");
    return;
  }
  backupPath += "/ishell_backup.data";
  ui->backupPath->setText(backupPath);
  QMessageBox::information(this, "提示", "密码、共私钥文件不参与备份");
  ConfirmDialog *confirmDialog = new ConfirmDialog(this, "请输入密码", "确定");
  confirmDialog->show();
  connect(confirmDialog, &ConfirmDialog::confirmEditEvent,
          [=](QString input) { backupFile(input); });
}

void BackupAndRecoveryDialog::on_recoveryButton_clicked() {
  QString recoveryPath = ui->recoveryFilePath->text();
  if (recoveryPath.isEmpty()) {
    QMessageBox::warning(this, "提示", "请选择恢复文件");
    return;
  }
  ConfirmDialog *confirmDialog = new ConfirmDialog(this, "请输入密码", "确定");
  confirmDialog->show();
  connect(confirmDialog, &ConfirmDialog::confirmEditEvent,
          [=](QString input) { recovery(input); });
}

void BackupAndRecoveryDialog::on_appendRecoveryButton_clicked() {
  QString recoveryPath = ui->recoveryFilePath->text();
  if (recoveryPath.isEmpty()) {
    QMessageBox::warning(this, "提示", "请选择恢复文件");
    return;
  }
  ConfirmDialog *confirmDialog = new ConfirmDialog(this, "请输入密码", "确定");
  confirmDialog->show();
  connect(confirmDialog, &ConfirmDialog::confirmEditEvent,
          [=](QString input) { appendRecovery(input); });
}

bool BackupAndRecoveryDialog::backupFile(QString password) {
  QList<ConnectInfo> infos = ConnectDao::GetInstance()->getConnectInfos();
  QString data = toJsonStr(infos);

  string key = password.toStdString();
  string encrypt_data = AES::aes_cbc(data.toStdString(), key, AES_ENCRYPT);
  QString backupPath = ui->backupPath->text();
  QFile file(backupPath);
  file.remove();
  if (file.open(QIODevice::WriteOnly)) {
    file.write(encrypt_data.data(), encrypt_data.length());
  }
  QMessageBox::information(this, "提示", "备份完成");
  return true;
}

bool BackupAndRecoveryDialog::recovery(QString password) {
  QString recoveryFilePath = ui->recoveryFilePath->text();
  QFile file(recoveryFilePath);
  bool isOpen = file.open(QIODevice::ReadOnly);
  if (!isOpen) {
    return false;
  }
  QByteArray array = file.readAll();
  string data = array.toStdString();
  string key = password.toStdString();
  string decrypt_data = AES::aes_cbc(data, key, AES_DECRYPT);
  int strSize = strlen(decrypt_data.data());
  decrypt_data = decrypt_data.substr(0, strSize);
  QString jsonStr = QString::fromStdString(decrypt_data);
  QJsonDocument document = QJsonDocument::fromJson(jsonStr.toUtf8());
  if (document.isObject()) {
    ConnectInfo info = ConnectInfo::jsonObjToConnectInfo(document.object());
    ConnectDao::GetInstance()->deleteAll();
    ConnectDao::GetInstance()->addConnectInfo(&info);

  } else if (document.isArray()) {
    ConnectDao::GetInstance()->deleteAll();
    for (auto v : document.array()) {
      ConnectInfo info = ConnectInfo::jsonObjToConnectInfo(v.toObject());
      ConnectDao::GetInstance()->addConnectInfo(&info);
    }
  }
  QMessageBox::information(this, "提示", "恢复完成");
  return true;
}

bool BackupAndRecoveryDialog::appendRecovery(QString password) {
  QString recoveryFilePath = ui->recoveryFilePath->text();
  QFile file(recoveryFilePath);
  bool isOpen = file.open(QIODevice::ReadOnly);
  if (!isOpen) {
    return false;
  }
  QByteArray array = file.readAll();
  string data = array.toStdString();
  string key = password.toStdString();
  string decrypt_data = AES::aes_cbc(data, key, AES_DECRYPT);
  int strSize = strlen(decrypt_data.data());
  decrypt_data = decrypt_data.substr(0, strSize);
  QString jsonStr = QString::fromStdString(decrypt_data);
  qDebug() << jsonStr;
  QJsonDocument document = QJsonDocument::fromJson(jsonStr.toUtf8());
  if (document.isObject()) {
    ConnectInfo info = ConnectInfo::jsonObjToConnectInfo(document.object());
    ConnectDao::GetInstance()->addConnectInfo(&info);

  } else if (document.isArray()) {
    QMap<int, QList<ConnectInfo> *> groupMapInfos;
    QList<ConnectInfo> groupInfos;
    for (auto v : document.array()) {
      ConnectInfo info = ConnectInfo::jsonObjToConnectInfo(v.toObject());
      if (info.parentId == 0) {
        QList<ConnectInfo> *list = groupMapInfos[info.id];
        if (list == NULL) {
          list = new QList<ConnectInfo>;
          groupMapInfos[info.id] = list;
        }
        groupInfos.append(info);
      } else {
        QList<ConnectInfo> *list = groupMapInfos[info.parentId];
        if (list == NULL) {
          list = new QList<ConnectInfo>;
          groupMapInfos[info.parentId] = list;
        }
        list->append(info);
      }
    }
    for (auto info : groupInfos) {
      int id = info.id;
      ConnectDao::GetInstance()->addConnectInfo(&info);
      int new_id = info.id;
      QList<ConnectInfo> *infos = groupMapInfos[id];
      for (auto info : *infos) {
        info.parentId = new_id;
        ConnectDao::GetInstance()->addConnectInfo(&info);
      }
    }
    qDebug() << "";
  }
  QMessageBox::information(this, "提示", "恢复完成");
  return true;
}
