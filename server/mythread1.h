#ifndef MYTHREAD1_H
#define MYTHREAD1_H
#include<QThread>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QString>
#include<QDebug>
#include<QQueue>
#include<QTimer>
//一个ZH三相电表，一个YC1002
class myThread1 : public QThread
{
    Q_OBJECT
public:
    explicit myThread1(QObject *parent = nullptr);
    void timeupdate();
protected:
    void run();//线程处理函数，不能直接调用，通过STRAT（）
    void readdata();
    void portConnect();
    void writePort();
public slots:
    void enSendQueue(QByteArray);
signals:
    void done();
    void transfer(QByteArray);
private:
    QSerialPort *port;
    QQueue<QByteArray>bufferSendQueue;
    QTimer *timer=new QTimer();
};

#endif // MYTHREAD1_H
