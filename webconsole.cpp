#include "webconsole.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>

#include "common.h"
#include "ui_webconsole.h"
#include "websocketserver.h"
WebConsole::WebConsole(QWidget *parent, ConnectInfo *connectInfo)
    : QWidget(parent), ui(new Ui::WebConsole) {
  ui->setupUi(this);

  this->connectInfo = *connectInfo;
  qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
  // qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();

  clientId = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

  webChannel = new QWebChannel(this);
  webChannel->registerObject(QStringLiteral("core"), this);

  webView = new QWebEngineView(this);
  webView->page()->setWebChannel(webChannel);
  webView->resize(parent->size());

  QFile file(":/html/xterm5/index.html");
  //  QFile file(":/html/xterm.html");
  if (!file.open(QIODevice::ReadOnly)) {
    return;
  }
  QString htmlData = file.readAll().constData();
  webView->setHtml(htmlData);

  //  webView->setUrl(QUrl("http://192.168.0.110:8080/"));
  webView->show();
  connect(webView, SIGNAL(loadFinished(bool)), this,
          SLOT(pageLoadFinished(bool)));
}

void WebConsole::resizeEvent(QResizeEvent *) { webView->resize(this->size()); }

void WebConsole::pageLoadFinished(bool flag) {
  webView->page()->runJavaScript(
      "setWebSocketServerPort(" +
      QString::number(
          WebSocketServer::getInstance()->getWebSocketServerPort()) +
      ")");
  webView->page()->runJavaScript("setClientId(" + clientId + ")");
  this->ssh2connect("");
}

void WebConsole::channelDataHandle(QString data) {
#ifdef ENABLE_WS
  WebSocketServer::getInstance()->sendQStringData(clientId, data);
#else
  QJsonObject obj;
  QJsonDocument doc;
  obj["data"] = data;
  doc.setObject(obj);
  data = doc.toJson(QJsonDocument::Compact);
  webView->page()->runJavaScript("xtermWrite(" + data + ")");
#endif
}

void WebConsole::connectSuccess() {
  webView->page()->runJavaScript("connectSuccess()");
}

void WebConsole::ssh2connect(const QString &jsMsg) {
  if (sshClient) {
    disconnect(sshClient);
    sshClient->stop();
    delete sshClient;
    sshClient = NULL;
  }

  if (connectInfo.authType == 1) {
    sshClient =
        new SSHClient(connectInfo.hostName, QString::number(connectInfo.port),
                      connectInfo.username, connectInfo.password);
  }

  if (connectInfo.authType == 2) {
    sshClient =
        new SSHClient(connectInfo.hostName, QString::number(connectInfo.port),
                      connectInfo.username, connectInfo.publicKeyPath,
                      connectInfo.privateKeyPath, connectInfo.passPhrase);
  }
  sshClient->pty_cols = cols;
  sshClient->pty_rows = rows;

  connect(sshClient, SIGNAL(connectSuccess()), this, SLOT(connectSuccess()));

  connect(sshClient, &SSHClient::errorMsg, this, [=](QString errMsg) {
    //    QMessageBox::warning(this, "错误提示", errMsg);
    AlertWindow *alertWindow = new AlertWindow(webView, true);
    alertWindow->setTitleText("错误提示");
    alertWindow->setContentText(errMsg + "\n是否重新连接");
    alertWindow->setConfirmButtonText("重连");
    connect(alertWindow, &AlertWindow::confirmEvent, this,
            [=]() { this->ssh2connect(""); });
    alertWindow->show();
    this->installEventFilter(alertWindow);
    //    close();
  });

  connect(sshClient, &SSHClient::disconnected, this, [=]() {
    AlertWindow *alertWindow = new AlertWindow(webView, true);
    alertWindow->setContentText("当前连接已关闭,是否重新连接");
    alertWindow->setConfirmButtonText("重连");
    connect(alertWindow, &AlertWindow::confirmEvent, this,
            [=]() { this->ssh2connect(""); });
    alertWindow->show();
    this->installEventFilter(alertWindow);
  });

  connect(sshClient, &SSHClient::openChannelSuccess, this,
          [=]() { openChannelSeccess = true; });

  connect(sshClient, &SSHClient::readChannelData, this,
          &WebConsole::channelDataHandle, Qt::DirectConnection);
  //  connect(sshClient, &SSHClient::readChannelData, this,
  //          &WebConsole::channelDataHandle, Qt::DirectConnection);
  sshClient->start();
}

void WebConsole::recieveJsMessage(const QString &shell) {
  sshClient->exec(shell);
  //    qDebug() << "input shell：" << shell;
}

void WebConsole::setChannelRequestPtySize(const QString &size) {
  QStringList ss = size.split(",");
  rows = ss[0].toInt();
  cols = ss[1].toInt();
  if (openChannelSeccess) {
    sshClient->setChannelRequestPtySize(rows, cols);
  }
}

void WebConsole::closeEvent(QCloseEvent *event) {
  webView->page()->runJavaScript("closews()");
  sshClient->stop();
}

void WebConsole::paintEvent(QPaintEvent *event) {
  //    Q_UNUSED(event);
  //    QPainter p(this);
  //    p.setPen(Qt::NoPen);
  //    p.setBrush(Qt::red);
  //    p.drawRect(rect());
}

WebConsole::~WebConsole() {
  delete ui;
  if (sshClient) {
    delete sshClient;
  }
  delete webView;
  delete webChannel;
}
