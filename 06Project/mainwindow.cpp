#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTextCodec"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status =false;//初始化连接状态
    connect(ui->connectButton,&QPushButton::clicked,this,&MainWindow::on_connectButton_clicked);
    //和服务器连接时发送连接信号connected
    connect(&tcpSocket,&QTcpSocket::connected,this,&MainWindow::onConnected);
    //断开连接时发送disconnected信号
    connect(&tcpSocket,&QTcpSocket::disconnected,this,&MainWindow::onDiconnected);
    //当服务器发送数据过来时发送readyRead信号
    connect(&tcpSocket,&QTcpSocket::readyRead,this,&MainWindow::slotDataReceived);
    //网络异常时发送error信号
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotSockError(QAbstractSocket::SocketError)));
    //消息发送按钮按下时的信号
    connect(ui->sendButton,&QPushButton::clicked,this,&MainWindow::on_sendButton_clicked);
}
//析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

//点击发送信息后的处理函数
 void MainWindow::on_sendButton_clicked()
 {
     //获取用户输入的信息
     if(ui->sendEdit->text()==""){
         return;
     }
     QString msg = userName + ":"+ ui->sendEdit->text();
     // 发送消息到服务器
     qDebug()<<"发送消息到服务器，消息为>" + msg;
     tcpSocket.write(msg.toLocal8Bit());
     //清空消息
     ui->sendEdit->clear();
 };

 void MainWindow::on_connectButton_clicked()
 {
     if(status ==false){
         QString ip = ui->serverIpEdit->text();
         if(serverIp.setAddress(ip) ==false){
             QMessageBox::information(this,"错误","请输入正确的IP地址");
             return;
         }
         if(ui->userNameEdit->text() ==""){
             QMessageBox::information(this,"错误","请输入聊天室昵称");
             return;
         }
         userName = ui->userNameEdit->text();
         if(ui->portEdit->text() ==""){
             QMessageBox::information(this,"错误","请输入正确的端口");
             return;
         }
         port = ui->portEdit->text().toShort();
         //连接服务器
         tcpSocket.connectToHost(serverIp,port);
         status = true;
     }
     else{
         //发送下线消息
        QString msg = userName+":离开聊天室";
        tcpSocket.write(msg.toLocal8Bit());
        //关闭服务器连接
        tcpSocket.disconnectFromHost();
        //修改标志位离线状态
        status = false;
    }
 };
//和服务器连接时的槽函数
 void MainWindow::onConnected(void){
    ui->sendButton->setEnabled(true);
    ui->connectButton->setText("离开服务器");
    //禁用ip、port、用户名 的输入框
    ui->serverIpEdit->setEnabled(false);
    ui->portEdit->setEnabled(false);
    ui->userNameEdit->setEnabled(false);
    QString msg = userName + ":进入聊天室";
    tcpSocket.write(msg.toLocal8Bit());

};
 //和服务器断开连接时调用的槽函数
 void MainWindow::onDiconnected(void){
     //禁用发送消息的按钮
     ui->sendButton->setEnabled(true);
     //ip,port应该是能够输入的状态
     ui->serverIpEdit->setEnabled(true);
     ui->portEdit->setEnabled(true);
     ui->userNameEdit->setEnabled(true);

     ui->connectButton->setText("连接服务器");

 };

 //接收聊天信息的槽函数
 void MainWindow::slotDataReceived(void)
 {
    if(tcpSocket.bytesAvailable())
    {
        QByteArray buf;
        buf.resize(tcpSocket.bytesAvailable());
        tcpSocket.read(buf.data(),buf.size());
        //将消息显示到界面
        //QString msg= buf.data();// QByteArray 转 QString 中文会乱码
         qDebug()<<"收到信息byte>"+buf;
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QString msg = codec->toUnicode(buf.data());
        qDebug()<<"收到信息>"+msg;
        qDebug()<<"收到信息>"+QString::fromUtf8(buf.data());
        ui->listWidget->addItem(msg);
    }
 };
 //网络异常处理的槽函数
 void MainWindow::slotSockError(QAbstractSocket::SocketError)
 {
     QMessageBox::critical(this,"网络异常",tcpSocket.errorString());
 };

