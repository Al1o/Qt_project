#ifndef MYTHREAD2_H
#define MYTHREAD2_H
#include <QThread>
#include<QDebug>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QString>
#include<QQueue>
#include<QTimer>
//三个YC1002
class myThread2 : public QThread
{
    Q_OBJECT
public:
    explicit myThread2(QObject *parent = nullptr);
    void timeupdate();

protected:
    void run();//线程处理函数，不能直接调用，通过STRAT（）
    void readdata();
    void portConnect();
    void writePort();
signals:
    void done();
    void transfer(QByteArray);
private:
    QQueue<QByteArray>bufferSendQueue;
    QSerialPort *port;
    QTimer *timer=new QTimer();

};

#endif // MYTHREAD2_H
