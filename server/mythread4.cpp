#include "mythread4.h"
//波形控制
mythread4::mythread4(QObject *parent) : QThread(parent)
{
    connect(timer,&QTimer::timeout,this,&mythread4::timeupdate);

}
QByteArray buf4;
void mythread4::timeupdate()
{
    timer->stop();
    emit transfer(buf4);
    qDebug()<<"buf3:"<<buf4;
    if(bufferSendQueue.isEmpty())
    {
        qDebug()<<"1添加指令";

    }
    buf4.clear();
    if(!bufferSendQueue.isEmpty())
        writePort();
}

void mythread4::run()
{
    qDebug()<<"Thread3 id="<<QThread::currentThreadId();
    port=new QSerialPort;
    portConnect();
    exec();
}

void mythread4::readdata()
{
    timer->start(820);
    buf4.append(port->readAll());
}

void mythread4::portConnect()
{
    port->setPortName("COM4");
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
    connect(port,&QSerialPort::readyRead,this,&mythread4::readdata);
    if(port->isOpen())
        emit done();

}

void mythread4::writePort()
{
    qDebug()<<"2写入操作指令";
    port->write(bufferSendQueue.dequeue());
    port->waitForReadyRead(1000);
}

void mythread4::enSendQueue(QByteArray sendToPortdata)
{

    bufferSendQueue.enqueue(sendToPortdata);
    if(!timer->isActive())
        writePort();
}
