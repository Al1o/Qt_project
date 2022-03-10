#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include<QHostAddress>
#include<QUdpSocket>
#include"mythread1.h"
#include"mythread2.h"
#include"mythread3.h"
#include"mythread4.h"
#include"mythread5.h"
#include<QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void tcp();
    void udp();
    void thread1();
    void receive1();
    void thread2();
    void thread3();
    void thread4();
    void thread5();
    void receive2();
    void receive3();
    void receive4();
    void receive5();
    void stopThread1();
    void stopThread2();
    void stopThread3();
    void stopThread4();
    uint16_t modbusCRC(QByteArray senddata);//CRC校验
    ~MainWindow();

private slots:
    void on_send_clicked();
    void newConnect();
    void socketReaddata();
    void socketDiscon();
    void on_listen_clicked();
    void senddata1(QByteArray);//线程1来的数据直接转发给客户端
    void senddata2(QByteArray);//线程2来的数据直接转发给客户端
    void udpSenddata(QByteArray);//UDP数据发送
    void udpReceive();
    void udpdataAnlysis(QByteArray buffer);//数据解析
    void on_tcpOn_clicked();
    void on_udpOn_clicked();
signals:
    void udpToPortOne(QByteArray);
    void udpToPortThree(QByteArray);
    void udpToPortFour(QByteArray);
    void udpToPortFive(QByteArray);
private:
    Ui::MainWindow *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    myThread1 *threadOne;
    myThread2 *threadTwo;
    mythread3 *threadThree;
    mythread4 *threadFour;
    mythread5 *threadFive;
    QTimer *timer;
    QUdpSocket *udpsender;

};
#endif // MAINWINDOW_H
