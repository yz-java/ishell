#ifndef QTERMWIDGET_H
#define QTERMWIDGET_H

#include "QTermScreen.h"
#include "sshclient.h"
#include "connectinfo.h"
class QTermWidget : public QTermScreen
{
    Q_OBJECT

public:
    QTermWidget(QWidget *parent = Q_NULLPTR,ConnectInfo* conInfo=Q_NULLPTR);
    ~QTermWidget();
    void setEcho(bool en);
    void setSendLine(bool en);

public slots:
    void putData(const QByteArray &data);
    void paste();
    void readChannelData(const char data);

signals:
    void outData(const QByteArray &data);

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void wheelEvent(QWheelEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void closeEvent(QCloseEvent *event);

private:
    void recvChar(char ch);
    void parseParam(QVector<int> &param, int np = 1, int defval = 0);
    void flushText();
    void debug(const QByteArray &data);
    void gbtou(const QByteArray &gb, QString &u);

private:
    void eraseText(char cmd);
    void moveCursor(char cmd);
    void setDisplay();
    void setTitle();

    SSHClient* sshClient;
    ConnectInfo connectInfo;
    bool connectSuccess=false;
    void ssh2connect();

private:
    int m_Mode;
    QString m_Param;
    QByteArray m_Text;
    QByteArray m_Line;
    bool m_Echo;
    bool m_SLine;
    int m_Cnt;
    bool ctrl_press;
};

#endif // QTERMWIDGET_H
