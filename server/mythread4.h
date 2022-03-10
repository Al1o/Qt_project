#ifndef MYTHREAD4_H
#define MYTHREAD4_H
#include <QThread>
#include<QDebug>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QString>
#include<QQueue>
#include<QTimer>
//波形发生控制
class mythread4 : public QThread
{
    Q_OBJECT
public:
    explicit mythread4(QObject *parent = nullptr);
    void timeupdate();

protected:
    void run();//线程处理函数，不能直接调用，通过STRAT（）
    void readdata();
    void portConnect();
    void writePort();
signals:
    void done();
    void transfer(QByteArray);
public slots:
    void enSendQueue(QByteArray);
private:
    QQueue<QByteArray>bufferSendQueue;
    QSerialPort *port;
    QTimer *timer=new QTimer();
};

#endif // MYTHREAD4_H
