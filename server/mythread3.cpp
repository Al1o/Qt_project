#include "mythread3.h"
//430开关控制
mythread3::mythread3(QObject *parent) : QThread(parent)
{
    connect(timer,&QTimer::timeout,this,&mythread3::timeupdate);

}
QByteArray buf3;
void mythread3::timeupdate()
{
    timer->stop();
    emit transfer(buf3);
    qDebug()<<"buf3:"<<buf3;
    if(bufferSendQueue.isEmpty())
    {
        qDebug()<<"1添加指令";

    }
    buf3.clear();
    if(!bufferSendQueue.isEmpty())
        writePort();
}

void mythread3::run()
{
    qDebug()<<"Thread3 id="<<QThread::currentThreadId();
    port=new QSerialPort;
    portConnect();
    exec();
}

void mythread3::readdata()
{
    timer->start(820);
    buf3.append(port->readAll());
}

void mythread3::portConnect()
{
    port->setPortName("COM2");
    //未完成，需要自动匹配此类处理的串口
//    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
//    {
//        port->setPort(info);
//    }
//    qDebug()<<port->portName();
    port->open(QIODevice::ReadWrite);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    connect(port,&QSerialPort::readyRead,this,&mythread3::readdata);
    if(port->isOpen())
        emit done();

}

void mythread3::writePort()
{
    qDebug()<<"2写入操作指令";
    port->write(bufferSendQueue.dequeue());
    port->waitForReadyRead(1000);
}

void mythread3::enSendQueue(QByteArray sendToPortdata)
{

    bufferSendQueue.enqueue(sendToPortdata);
    if(!timer->isActive())
        writePort();
}
