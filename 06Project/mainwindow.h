#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QDialog"
#include "QHostAddress"
#include "QTcpSocket"
#include "QMessageBox"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //定义槽函数
    //发送信息，按钮的槽函数
    void on_sendButton_clicked();
    //连接服务器，按钮的槽函数
    void on_connectButton_clicked();

    //和服务器连接时调用的槽函数
    void onConnected(void);
    //和服务器断开连接时调用的槽函数
    void onDiconnected(void);
    //接收聊天信息的槽函数
    void slotDataReceived(void);
    //网络异常处理的槽函数
    void slotSockError(QAbstractSocket::SocketError);


    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool status;//标记客户端的连接状态，true在线，false离线
    QTcpSocket tcpSocket;//和服务器通信的套接字
    qint16 port;//服务器通信端口
    QHostAddress serverIp;//服务器ip
    QString userName;//聊天室昵称

};
#endif // MAINWINDOW_H
