#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->createButton,&QPushButton::clicked,this,&MainWindow::on_createButton_clicked);
    connect(&tcpServer,&QTcpServer::newConnection,this,&MainWindow::onNewConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//创建TCP服务器
void MainWindow::on_createButton_clicked()
{
    qDebug() << "准备执行TCP服务器创建逻辑";
    port = ui->portEdit->text().toShort();
    //开启服务器
    if(tcpServer.listen(QHostAddress::Any,port)){
        qDebug("TCP服务器创建成功");
    }
    else{
         qDebug("TCP服务器创建失败");
    }
    //禁用创建按钮和端口输入
    ui->createButton->setEnabled(false);
    ui->portEdit->setEnabled(false);
}
//当客户端和服务器建立链接发送信号newConnection
void MainWindow::onNewConnection()
{
    //获取和客户端通信的套接字
    QTcpSocket* tcpClientSocket =
            tcpServer.nextPendingConnection();
    //保存套接字到容器
    tcpClientList.append(tcpClientSocket);


    //收到数据时，将会发送readyRead
    connect(tcpClientSocket,&QTcpSocket::readyRead,
                this,&MainWindow::slotDataReceived);
}
//连接readyRead信号
void MainWindow::slotDataReceived()
{
    //遍历所有的客户端
    for(int i=0; i<tcpClientList.count(); i++){
        //检查当前客户端是否有数据到来
        if(tcpClientList.at(i)->bytesAvailable()){
            //接收客户端发送来的消息
            QByteArray readbuf =
                tcpClientList.at(i)->readAll();
            qDebug()<<"接收到客户端发送过来的信息>:"+readbuf;
            QString msg = readbuf.data();
            qDebug()<<"接收到客户端发送过来的信息>:"+msg;
            //显示到服务器Ui
            ui->listWidget->addItem(readbuf.data());
            //转发消息给所有的客户端
            sendMessage(readbuf);
        }
    }
}
//转发消息给所有的客户端
void MainWindow::sendMessage(const QByteArray& msg)
{
    for(int i=0;i<tcpClientList.count();i++){
        tcpClientList.at(i)->write(msg);
    }
}
