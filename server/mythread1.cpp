#include "mythread1.h"
bool enablesend=true;//正常轮询访问寄存器
int highersend=0;//需要发送的指令数
QByteArray sendData1;//ZH 地址01
QByteArray sendData2;//YC 地址02
myThread1::myThread1(QObject *parent) : QThread(parent)
{
    connect(timer,&QTimer::timeout,this,&myThread1::timeupdate);
}
QByteArray buf;

void myThread1::run()
{
    qDebug()<<"Thread1 id="<<QThread::currentThreadId();
    port=new QSerialPort;
    portConnect();
    exec();

}
//延迟读取一个数据帧前后各加3.5字节时间
void myThread1::readdata()
{
    timer->start(820);
    buf.append(port->readAll());
}

void myThread1::timeupdate()
{
//    qDebug()<<"T1 read data";
    timer->stop();
    emit transfer(buf);
//    qDebug()<<"buf1"<<buf;
    if(bufferSendQueue.isEmpty())
    {
//        qDebug()<<"1添加指令";
        bufferSendQueue.enqueue(sendData1);
        bufferSendQueue.enqueue(sendData2);
    }
    buf.clear();

}
//发送数据本身经过CRC校验暂时无用
uint16_t modbusCRC(QByteArray senddata)
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
void senddress1()
{

}
void senddress2()
{

}

int i=1;
void myThread1::portConnect()
{
    qDebug()<<"port id="<<QThread::currentThreadId();
//    port=new QSerialPort;
    port->setPortName("COM3");
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
    connect(port,&QSerialPort::readyRead,this,&myThread1::readdata);
    if(port->isOpen())
        emit done();

//sendData1-ZH4041

    sendData1.resize(8);//数据第一位是地址，从第0位寄存器读48个
    sendData1[0]=0x0E;sendData1[1]=0x03;sendData1[2]=0x00;sendData1[3]=0x00;sendData1[4]=0x00;sendData1[5]=0x2F;
    uint16_t CRC=modbusCRC(sendData1);
    uint16_t crcF=CRC%256;
    uint16_t crcB=(CRC-crcF)/256;
    sendData1[6]=crcF;sendData1[7]=crcB;
//sendData2—YC1002B

    sendData2.resize(8);//数据第一位是地址，从第32位寄存器读32个
    sendData2[0]=0x0F;sendData2[1]=0x03;sendData2[2]=0x00;sendData2[3]=0x20;sendData2[4]=0x00;sendData2[5]=0x20;
    CRC=modbusCRC(sendData2);
    crcF=CRC%256;
    crcB=(CRC-crcF)/256;
    sendData2[6]=crcF;sendData2[7]=crcB;
//sendData2—YC1002B
    QByteArray sendData3;
    sendData3.resize(8);//数据第一位是地址，从第32位寄存器读32个
    sendData3[0]=0x03;sendData3[1]=0x03;sendData3[2]=0x00;sendData3[3]=0x20;sendData3[4]=0x00;sendData3[5]=0x20;
    CRC=modbusCRC(sendData3);
    crcF=CRC%256;
    crcB=(CRC-crcF)/256;
    sendData3[6]=crcF;sendData3[7]=crcB;
    bufferSendQueue.enqueue(sendData1);
    bufferSendQueue.enqueue(sendData2);
//    writePortAlltime();
    for(;;)//循环发送读取设备寄存器信号
    {
        if(!bufferSendQueue.isEmpty())
        {
            writePort();
        }
        else {

        }
//        if(enablesend&&highersend==0){
//        port->write(sendData2);
//        enablesend=false;
//        port->waitForReadyRead(1000);
//        }
//        if(enablesend&&highersend==0){
//        port->write(sendData1);
//        enablesend=false;
//        port->waitForReadyRead(1000);
//        }
//        if(enablesend){
//        port->write(sendData3);
//        enablesend=false;
//        port->waitForReadyRead(1000);
//        enablesend=true;
//        }
    }
}
//向指令队列中加入操作指令
void myThread1::writePort()
{

//    qDebug()<<"1写入操作指令";
    port->write(bufferSendQueue.dequeue());
    port->waitForReadyRead(1000);
//    highersend--;
}

//void myThread1::writePortAlltime()
//{
//    qDebug()<<"进入读取数据函数"<<i;
//    if(i==1)
//    {
//        qDebug()<<"进入1";
//        port->write(sendData1);
//        port->waitForReadyRead(1000);
//        i=2;
//    }
//    else if(i==2)
//    {
//        qDebug()<<"进入2";
//        port->write(sendData2);
//        port->waitForReadyRead(1000);
//        i=1;
//    }
//}

void myThread1::enSendQueue(QByteArray sendToPortdata)
{
//    highersend++;
    bufferSendQueue.clear();
    bufferSendQueue.enqueue(sendToPortdata);
    qDebug()<<"1触发一个信号";
//    if(!timer->isActive())
//    {
//        writePort();
//    }

}

//2021/3/24：串口连接有问题，显示连接无发送无接收 已解决：port->waitForReadyRead(1000);
//2021/3/30；串口建立以及发送在从线程，类声明的其他函数在主线程中运行（readdata）。问题：多个串口读取数据是否引发数据错乱。
//2021/3/31：第一次尝试两台设备收发信息，顺序发送，可以通信，收回数据有错乱。设置bool变量为锁完整收到数据后再开启发送权限
//2021/4/1：CRC校验成功，两台设备在一个线程中通过服务器并经过CRC校验后传到客户端上。
//2021/4/3：三台设备同一串口同一线程通信，只能与两台设备通信。
           //第二次测试成功。
//解决数据发送冲突问题：添加缓冲区，一个队列，在for循环中将串口指令添加到队列尾然后依次发送，当服务器来操作指令时，将操作指令置于队列头；2021/5/10
//数据发送冲突问题已解决，用队列建立缓冲区。
