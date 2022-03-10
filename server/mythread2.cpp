#include "mythread2.h"
bool enablesend2=true;
QByteArray sendData0B;//YC 地址0B
QByteArray sendData0C;//YC 地址0C
QByteArray sendData0D;//YC 地址0D
myThread2::myThread2(QObject *parent) : QThread(parent)
{
    connect(timer,&QTimer::timeout,this,&myThread2::timeupdate);

}
QByteArray buf2;

void myThread2::timeupdate()
{
//    qDebug()<<"T2 read data";
    timer->stop();
//    enablesend2=true;
    emit transfer(buf2);
    if(bufferSendQueue.isEmpty())
    {
//        qDebug()<<"2添加指令";
        bufferSendQueue.enqueue(sendData0B);
        bufferSendQueue.enqueue(sendData0C);
        bufferSendQueue.enqueue(sendData0D);
    }
    buf2.clear();
}

void myThread2::run()
{
//    qDebug()<<"Thread2 id="<<QThread::currentThreadId();
    port=new QSerialPort;
    portConnect();
    exec();

}

void myThread2::readdata()
{
    timer->start(820);
    buf2.append(port->readAll());
}
uint16_t modbusCRC2(QByteArray senddata)
{
    int len=senddata.size()-2;
    uint16_t wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1
    uint8_t temp;//定义中间变量
    int i=0,j=0;//定义计数
    for(i=0;i<len;i++)//循环计算每个数据
    {
       temp=senddata.at(i);
       wcrc^=temp;
       for(j=0;j<8;j++){
          //判断右移出的是不是1，如果是1则与多项式进行异或。
          if(wcrc&0X0001){
              wcrc>>=1;//先将数据右移一位
              wcrc^=0XA001;//与上面的多项式进行异或
          }
          else//如果不是1，则直接移出
              wcrc>>=1;//直接移出
       }
    }
    temp=wcrc;//crc的值
    return wcrc;
}
void myThread2::portConnect()
{
    //    port=new QSerialPort;
        port->setPortName("COM5");
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
        connect(port,&QSerialPort::readyRead,this,&myThread2::readdata);
        if(port->isOpen())
            emit done();
    //sendData1-YC
        sendData0B.resize(8);//数据第一位是地址，
    //    sendData1[0]=0x01;sendData1[1]=0x03;sendData1[2]=0x00;sendData1[3]=0x00;sendData1[4]=0x00;sendData1[5]=0x30;
        sendData0B[0]=0x0B;sendData0B[1]=0x03;sendData0B[2]=0x00;sendData0B[3]=0x20;sendData0B[4]=0x00;sendData0B[5]=0x1C;
        uint16_t CRC=modbusCRC2(sendData0B);
        uint16_t crcF=CRC%256;
        uint16_t crcB=(CRC-crcF)/256;
        sendData0B[6]=crcF;sendData0B[7]=crcB;
    //sendData2—YC1002B
        sendData0C.resize(8);//数据第一位是地址，从第32位寄存器读32个
        sendData0C[0]=0x0C;sendData0C[1]=0x03;sendData0C[2]=0x00;sendData0C[3]=0x20;sendData0C[4]=0x00;sendData0C[5]=0x1C;
        CRC=modbusCRC2(sendData0C);
        crcF=CRC%256;
        crcB=(CRC-crcF)/256;
        sendData0C[6]=crcF;sendData0C[7]=crcB;
    //sendData2—YC1002B
        sendData0D.resize(8);//数据第一位是地址，从第32位寄存器读32个
        sendData0D[0]=0x0D;sendData0D[1]=0x03;sendData0D[2]=0x00;sendData0D[3]=0x20;sendData0D[4]=0x00;sendData0D[5]=0x1C;
        CRC=modbusCRC2(sendData0D);
        crcF=CRC%256;
        crcB=(CRC-crcF)/256;
        sendData0D[6]=crcF;sendData0D[7]=crcB;
        bufferSendQueue.enqueue(sendData0B);
        bufferSendQueue.enqueue(sendData0C);
        bufferSendQueue.enqueue(sendData0D);
        for(;;)//循环发送读取设备寄存器信号
        {
            if(!bufferSendQueue.isEmpty())
            {
                writePort();
            }
            else {

            }
        }
}

void myThread2::writePort()
{
//    qDebug()<<"2写入操作指令";
    port->write(bufferSendQueue.dequeue());
    port->waitForReadyRead(1000);
}
