#include "mainwindow.h"
#include "ui_mainwindow.h"
bool tcpsend=true;
MainWindow:: MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer;
    qDebug()<<"Threadmain id="<<QThread::currentThreadId();
    thread1();//串口连接线程1
    thread2();//串口连接线程2
    thread3();
    thread4();
    thread5();
}

void MainWindow::tcp()
{
    ui->port->setText("8080");
    ui->send->setEnabled(false);
    server=new QTcpServer();
    connect(server,&QTcpServer::newConnection,this,&MainWindow::newConnect);
    connect(this,&MainWindow::udpToPortOne,threadOne,&myThread1::enSendQueue);//
    connect(this,&MainWindow::udpToPortThree,threadThree,&mythread3::enSendQueue);//
    connect(this,&MainWindow::udpToPortFour,threadFour,&mythread4::enSendQueue);//
    connect(this,&MainWindow::udpToPortFive,threadFive,&mythread5::enSendQueue);
}

void MainWindow::udp()
{
    //发送端
    udpsender=new QUdpSocket();
//    ui->udpport->setText("8088");
    connect(threadOne,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
    connect(threadTwo,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
    connect(threadThree,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
    connect(threadFour,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
    //接收端
    connect(this,&MainWindow::udpToPortOne,threadOne,&myThread1::enSendQueue);
    connect(this,&MainWindow::udpToPortThree,threadThree,&mythread3::enSendQueue);
    connect(this,&MainWindow::udpToPortFour,threadFour,&mythread4::enSendQueue);
    udpsender->bind(QHostAddress("127.0.0.1"),8888);
    connect(udpsender,SIGNAL(readyRead()),this,SLOT(udpReceive()));
//    udpReceiver=new QUdpSocket();
//    udpReceiver->bind(QHostAddress("127.0.0.2"),8088);
//    connect(udpReceiver,SIGNAL(readyRead()),this,SLOT(udpReceive()));

}
//创建线程1
void MainWindow::thread1()
{
    threadOne=new myThread1(this);
    threadOne->start();
    connect(threadOne,&myThread1::done,this,&MainWindow::receive1);
    connect(this,&MainWindow::destroy,this,&MainWindow::stopThread1);//关闭窗口时触发

}
//线程内部处理完接收函数
void MainWindow::receive1()
{
    qDebug()<<"串口已连接";
}

void MainWindow::thread2()
{
    threadTwo=new myThread2(this);
    threadTwo->start();
    connect(threadTwo,&myThread2::done,this,&MainWindow::receive2);
}

void MainWindow::thread3()
{
    threadThree=new mythread3(this);
    threadThree->start();
    connect(threadThree,&mythread3::done,this,&MainWindow::receive3);
}

void MainWindow::thread4()
{
    threadFour=new mythread4(this);
    threadFour->start();
    connect(threadFour,&mythread4::done,this,&MainWindow::receive4);
}

void MainWindow::thread5()
{
    threadFive=new mythread5(this);
    threadFive->start();
    connect(threadFive,&mythread5::done,this,&MainWindow::receive5);
  //  connect(threadFive->server2,&QTcpServer::newConnection,this,&mythread5::newTcpconnect);
}

void MainWindow::receive2()
{
    qDebug()<<"线程2";
}

void MainWindow::receive3()
{
    qDebug()<<"线程3";
}

void MainWindow::receive4()
{
    qDebug()<<"线程4";
}

void MainWindow::receive5()
{
    qDebug()<<"线程5";
}
//停止线程
void MainWindow::stopThread1()
{
    threadOne->quit();//停止线程
    threadOne->wait();//等待线程处理完工作
    qDebug()<<"线程1已关闭";
}

void MainWindow::stopThread2()
{
    threadTwo->quit();
    threadTwo->wait();
    qDebug()<<"线程2已关闭";
}

void MainWindow::stopThread3()
{
    threadThree->quit();
    threadThree->wait();
    qDebug()<<"线程3已关闭";
}

void MainWindow::stopThread4()
{
    threadFour->quit();
    threadFour->wait();
    qDebug()<<"线程4已关闭";
}

uint16_t MainWindow::modbusCRC(QByteArray senddata)
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
MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_listen_clicked()
{
    if(ui->listen->text()=="侦听")
    {
        int port=ui->port->text().toInt();
        //监听指定端口
        if(!server->listen(QHostAddress::Any,port))
        {
            qDebug()<<server->errorString();
            return;
        }
        ui->listen->setText("取消侦听");
        qDebug()<<"侦听成功";
    }
    else
    {
        //取消侦听
        if(ui->labelCon->text()=="已连接"){
        server->close();
        socket->abort();
        }
        else{
            server->disconnect();
            server->close();
        }
        ui->listen->setText("侦听");
        ui->send->setEnabled(false);
    }
}
//接受到自线程发送来的数据进行CRC校验后通过TCP转发到客户端
void MainWindow::senddata1(QByteArray data)
{
    uint16_t CRC=modbusCRC(data);
    uint8_t crcF=CRC%256;
    uint8_t crcB=(CRC-crcF)/256;
    uint8_t a=(uint8_t)data[data.size()-2];
    uint8_t b=(uint8_t)data[data.size()-1];
//    qDebug()<<"CRCF:"<<hex<<crcF;
//    qDebug()<<"CRCB:"<<hex<<crcB;
//    qDebug()<<"dataF"<<hex<<a;
//    qDebug()<<"dataB"<<hex<<b;
    if((crcF==a)&&(crcB==b))
    {
        while (!tcpsend) {

        }
        tcpsend=false;
        socket->write(data);
        timer->start(500);
        tcpsend=true;
//        qDebug()<<"CRC校验成功";
    }
    else
    {
//        qDebug()<<"CRC校验失败";
    }

}
void MainWindow::senddata2(QByteArray data)//重写，改成事件信息传输
{
    uint16_t CRC=modbusCRC(data);
    uint8_t crcF=CRC%256;
    uint8_t crcB=(CRC-crcF)/256;
    uint8_t a=(uint8_t)data[data.size()-2];
    uint8_t b=(uint8_t)data[data.size()-1];
//    qDebug()<<"CRCF:"<<hex<<crcF;
//    qDebug()<<"CRCB:"<<hex<<crcB;
//    qDebug()<<"dataF"<<hex<<a;
//    qDebug()<<"dataB"<<hex<<b;
    if((crcF==a)&&(crcB==b))
    {
        while (!tcpsend) {

        }
        tcpsend=false;
        socket->write(data);
        timer->start(500);
        tcpsend=true;
//        qDebug()<<"CRC校验成功";
    }
    else
    {
//        qDebug()<<"CRC校验失败";
    }

}

void MainWindow::udpSenddata(QByteArray data)
{
    udpsender->writeDatagram(data,QHostAddress("127.0.0.1"),ui->udpport->text().toInt());
    qDebug()<<"udp 发送"<<data.toHex();

}
//从客户端接收到操作指令发送到子线程再发送到下位机
void MainWindow::udpReceive()
{
    QByteArray data;
    while(udpsender->hasPendingDatagrams())
    {
        data.resize(udpsender->pendingDatagramSize());
        udpsender->readDatagram(data.data(),data.size());
        ui->textReceive->append(data.toHex());
        udpdataAnlysis(data);
        //分析数据地址向下位机发送
    }
}

void MainWindow::udpdataAnlysis(QByteArray buffer)
{
    uint8_t station=buffer[0];
    qDebug()<<"station:"<<station;
    if(station==0x0E||station==0x0F)
    {
        qDebug()<<"发送到子线程1";
        ui->textReceive->append(buffer.toHex());
        udpToPortOne(buffer);
    }
    else if(station==0x0B||station==0x0C||station==0x0D)
    {

    }
    else if(station==0x3A)
    {
        qDebug()<<"发送到子线程3";
        ui->textReceive->append(buffer.toHex());
        udpToPortThree(buffer);
    }
    else if(station==0x24)
    {
        qDebug()<<"发送到子线程4";
        ui->textReceive->append(buffer.toHex());
        udpToPortFour(buffer);
    }
    else if(station==0x40)
    {

        qDebug()<<"发送到子线程5";
        ui->textReceive->append(buffer.mid(1));
        udpToPortFive(buffer.mid(1));

    }
}
//void MainWindow::udpReceive()
//{
//    QByteArray data;
//    while(udpReceiver->hasPendingDatagrams())
//    {
//        data.resize(udpReceiver->pendingDatagramSize());
//        udpReceiver->readDatagram(data.data(),data.size());
//        qDebug()<<"udp 接收"<<data.toHex();
//        //分析数据地址向下位机发送
//    }
//}
void MainWindow::on_send_clicked()
{
//转化为ASCLL发送
    qDebug()<<ui->textSend->toPlainText();
    socket->write(ui->textSend->toPlainText().toLatin1());
    socket->flush();
}

void MainWindow::newConnect()
{
    socket=server->nextPendingConnection();//与客户端连接
    QObject::connect(socket,&QTcpSocket::readyRead,this,&MainWindow::socketReaddata);
    QObject::connect(socket,&QTcpSocket::disconnected,this,&MainWindow::socketDiscon);
    connect(threadOne,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    connect(threadTwo,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    connect(threadThree,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    connect(threadFour,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    connect(threadFive,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    //connect(this,SIGNAL(udpToPortOne(QByteArray)),threadOne,SLOT(writePort(QByteArray)));

    ui->send->setEnabled(true);
    qDebug()<<"连接上客户端";
    ui->labelCon->setText("已连接");
    ui->send->setEnabled(true);
}

void MainWindow::socketReaddata()
{
    QByteArray buffer;
    buffer=socket->readAll();
    if(!buffer.isEmpty())
    {

        udpdataAnlysis(buffer);
    }
}

void MainWindow::socketDiscon()
{
    ui->send->setEnabled(false);
    qDebug()<<"客户端连接已断开";
    ui->labelCon->setText(" ");//客户端连接指示
    disconnect(threadOne,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    disconnect(threadTwo,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    disconnect(threadThree,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    disconnect(threadFour,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
    disconnect(threadFive,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
}
//问题：连接多次以后多次进入发送函数  解决：在断开网络连接时断开connect函数
//2021/4/13：多串口测试问题：两个串口可能同时使用TCP通信导致客户端接收到两个拼接在一起的数据帧。




void MainWindow::on_tcpOn_clicked()
{
    if(ui->tcpOn->text()=="开启TCP")
    {
        tcp();
        ui->listen->setEnabled(true);
        ui->tcpOn->setText("关闭TCP");
    }
    else
    {
       if(ui->labelCon->text()=="已连接"){
           disconnect(threadOne,SIGNAL(transfer(QByteArray)),this,SLOT(senddata1(QByteArray)));
           disconnect(threadTwo,SIGNAL(transfer(QByteArray)),this,SLOT(senddata2(QByteArray)));
           disconnect(threadThree,SIGNAL(transfer(QByteArray)),this,SLOT(senddata2(QByteArray)));
           disconnect(threadFour,SIGNAL(transfer(QByteArray)),this,SLOT(senddata2(QByteArray)));
           server->close();
           socket->abort();
       }
       ui->listen->setEnabled(false);
       ui->tcpOn->setText("开启TCP");
       ui->listen->setText("侦听");
       ui->send->setEnabled(false);
    }
}

void MainWindow::on_udpOn_clicked()
{
    if(ui->udpOn->text()=="开启发送")
    {
        udp();
        ui->udpOn->setText("关闭发送");
    }
    else
    {
        ui->udpOn->setText("开启发送");
        disconnect(threadOne,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
        disconnect(threadTwo,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
        disconnect(threadThree,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
        disconnect(threadFour,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
    }
}
