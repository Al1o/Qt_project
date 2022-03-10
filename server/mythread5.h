#ifndef MYTHREAD5_H
#define MYTHREAD5_H
#include <QThread>
#include<QDebug>
#include<QString>
#include<QQueue>
#include<QTimer>
#include<QTcpServer>
#include<QTcpSocket>
#include<QHostAddress>
//7606
class mythread5 : public QThread
{
    Q_OBJECT
public:
    explicit mythread5(QObject *parent = nullptr);
    void timeupdate();
    void newTcpconnect();
    void tcpReadData();
    void tcpSendData();
    void socketDiscon();
    QTcpServer *server2;
protected:
    void run();//
    void Tcpconnect();
signals:
    void transfer(QByteArray);
    void done();
public slots:
    void enSendQueue(QByteArray);
private:

    QTcpSocket *socket2;
    QQueue<QByteArray>bufferSendQueue;
    QTimer *timer=new QTimer();
};
#endif // MYTHREAD5_H
