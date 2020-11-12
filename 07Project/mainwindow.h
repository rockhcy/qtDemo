#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QDialog"
#include "QTcpServer"
#include "QTcpSocket"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void on_createButton_clicked();
    void onNewConnection();
    void slotDataReceived();

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void sendMessage(const QByteArray&);
    QTcpServer tcpServer;
    qint16 port;
    QList <QTcpSocket*> tcpClientList;

};
#endif // MAINWINDOW_H
