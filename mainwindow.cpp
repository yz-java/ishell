#include "mainwindow.h"

#include <QDir>
#include <QFile>
#include <QHostInfo>
#include <QKeyEvent>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QScreen>

#include "db/dbutil.h"
#include "sftpdialog.h"
#include "ui_mainwindow.h"
#include "vncviewerwidget.h"
#include "vncwebviewwdiget.h"
#include "webconsole.h"
#include "websocketserver.h"
extern "C" {
#ifdef XFREE_RDP
//#include "xfreerdp/xfreerdp.h"
#include "xfree-rdp/xfreerdp.h"
#endif
}

MainWindow *mainwindow = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("ishell");
  setWindowIcon(QIcon(":/logo.png"));
  qDebug() << "main ThreadId is" << QThread::currentThreadId();

  QThread *t = new QThread;
  auto wsServer = WebSocketServer::getInstance();
  wsServer->moveToThread(t);
  connect(t, &QThread::started, wsServer, &WebSocketServer::start);
  t->start();

  setMinimumSize(800, 600);
  //  setMaximumSize(1280, 720);
  //  setMinimumSize(1920, 1080);

  QDir dir;
  dir.mkdir(Common::workspacePath);

  DBUtil::GetInstance()->init();

  QTimer::singleShot(100, this, [=]() { initUI(); });
}

void MainWindow::initUI() {
  connectManagerUI = new ConnectManagerUI(this);

  ui->tabWidget->setTabPosition(QTabWidget::North);
  ui->tabWidget->insertTab(0, connectManagerUI, QIcon(":/icons/manager.png"),
                           "资产管理");
  ui->tabWidget->setTabToolTip(0, "资产管理");

  ui->tabWidget->setUsesScrollButtons(true);
  ui->tabWidget->insertTab(1, new welcomewidget(this),
                           QIcon(":/icons/welcome.png"), "欢迎页");
  ui->tabWidget->setTabToolTip(1, "欢迎页");
  this->ui->tabWidget->setTabsClosable(true);
  ui->tabWidget->setCurrentIndex(0);
  mainwindow = this;
  connect(connectManagerUI, SIGNAL(openSSHConnect(ConnectInfo)), this,
          SLOT(openSSHConnect(ConnectInfo)));
  connect(connectManagerUI, SIGNAL(openVNCConnect(ConnectInfo)), this,
          SLOT(openVNCConnect(ConnectInfo)));
  connect(connectManagerUI, SIGNAL(openRDPConnect(ConnectInfo)), this,
          SLOT(openRDPConnect(ConnectInfo)));
  connect(connectManagerUI, SIGNAL(openSFTPConnect(ConnectInfo)), this,
          SLOT(openSFTPConnect(ConnectInfo)));
  connect(this, &MainWindow::alertErrorMessageBox, this,
          [=](QString msg) { QMessageBox::warning(this, "错误提示", msg); });
}

void MainWindow::resizeEvent(QResizeEvent *) {
  this->ui->tabWidget->resize(this->size());
}

void MainWindow::keyPressEvent(QKeyEvent *e) {}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_tabWidget_currentChanged(int index) {
  qDebug() << index;
  currentIndex = index;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
  qDebug() << "close ==> " << index;
  if (index == 0) {
    return;
  }
  QString type = ui->tabWidget->widget(index)->property("type").toString();
  if (type == "vnc") {
#ifdef USE_NOVNC_CLIENT
    VncWebViewWdiget *w = (VncWebViewWdiget *)ui->tabWidget->widget(index);
    w->close();
#else
    QWidget *w = ui->tabWidget->widget(index);
    w->close();
#endif
  }
  if (type == "ssh") {
    WebConsole *console = (WebConsole *)ui->tabWidget->widget(index);
    console->close();
  }

  ui->tabWidget->removeTab(index);
}

void MainWindow::openSSHConnect(ConnectInfo connectInfo) {
  int count = ui->tabWidget->count();
  ui->tabWidget->insertTab(count, new WebConsole(this, &connectInfo),
                           QIcon(":/icons/console.png"),
                           connectInfo.name + "-ssh");

  ui->tabWidget->setTabToolTip(count, connectInfo.hostName);
  ui->tabWidget->setCurrentIndex(count);
  ui->tabWidget->widget(count)->setProperty("type", "ssh");
}

static void vncErrorCallback(char *msg) {
  mainwindow->alertErrorMessageBox(QString::fromUtf8(msg));
}

void MainWindow::openVNCConnect(ConnectInfo connectInfo) {
  if (connectInfo.hostName.isEmpty() || connectInfo.vncPassword.isEmpty() ||
      connectInfo.vncPort == 0) {
    QMessageBox::warning(this, "错误", "VNC认证配置错误");
    return;
  }
  int count = ui->tabWidget->count();
#ifdef USE_NOVNC_CLIENT
  ui->tabWidget->insertTab(count, new VncWebViewWdiget(this, connectInfo),
                           QIcon(":/icons/desktop.png"),
                           connectInfo.name + "-vnc");
#else
  QWidget *w = new QWidget();
  QHBoxLayout *centralLayout = new QHBoxLayout(w);
  centralLayout->setAlignment(Qt::AlignCenter);
  centralLayout->setMargin(0);
  VncViewerWidget *viewerWidget = new VncViewerWidget(w, connectInfo);
  centralLayout->addWidget(viewerWidget);
  w->setLayout(centralLayout);
  w->installEventFilter(viewerWidget);
  ui->tabWidget->insertTab(count, w, QIcon(":/icons/desktop.png"),
                           connectInfo.name + "-vnc");
#endif
  ui->tabWidget->setTabToolTip(count, connectInfo.hostName);
  ui->tabWidget->setCurrentIndex(count);
  ui->tabWidget->widget(count)->setProperty("type", "vnc");
}

void MainWindow::openRDPConnect(ConnectInfo connectInfo) {
  if (connectInfo.hostName.isEmpty() || connectInfo.rdpUserName.isEmpty() ||
      connectInfo.rdpPassword.isEmpty() || connectInfo.rdpPort == 0) {
    QMessageBox::warning(this, "错误", "RDP认证配置错误");
    return;
  }
  std::thread t([=]() {
    QHostInfo info = QHostInfo::fromName(connectInfo.hostName);
    QString hostName = info.addresses().first().toString();
    string host_name = hostName.toStdString();
    char *host = (char *)host_name.data();
    string userName = connectInfo.rdpUserName.toStdString();
    char *un = (char *)userName.data();
    string password = connectInfo.rdpPassword.toStdString();
    char *pwd = (char *)password.data();
    int port = connectInfo.rdpPort;
#ifdef XFREE_RDP
    openRDPViewer(host, un, pwd, port, vncErrorCallback);
#endif
  });
  t.detach();
}

void MainWindow::openSFTPConnect(ConnectInfo connectInfo) {
  SftpDialog *sftpDialog = new SftpDialog(this, connectInfo);
  //  sftpDialog->show();
  int count = ui->tabWidget->count();
  ui->tabWidget->insertTab(count, sftpDialog, QIcon(":/icons/folder.png"),
                           connectInfo.name + "-文件管理");
  ui->tabWidget->setTabToolTip(count, connectInfo.name + "-文件管理");
  ui->tabWidget->setCurrentIndex(count);
}
