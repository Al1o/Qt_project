#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<mainwindow.h>
#include<QWidget>
#include<QObject>
#include<QPalette>
#include"logging.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    interfaceControl();
    tcp();
    udp();
    dataB();//数据库
    electricBox();//电柜
    timer=new QTimer(this);
    QObject::connect(timer,&QTimer::timeout,this,&MainWindow::time);
    timer->start(10);
}
MainWindow::~MainWindow()
{
    delete this->tcpClient;
    delete ui;
}

//数据库
void MainWindow::dataB()
{
    dataBase=QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("dataBase.db");
    dataBase.open();
    sql=new QSqlQuery();

}
void MainWindow::on_log_clicked()
{
    if(ui->log->text()=="用户登录"){
        log.show();
    }
    else if(ui->log->text()=="注销用户")
    {
        ui->log->setText("用户登录");
        ui->limitLabel->setText("");
        ui->levelLabel->setText("");
        logging::uservalue=0;
    }
}
void MainWindow::usercheck()
{
    ui->username->setText(logging::userNow);
    if(logging::uservalue==0)
    {
        ui->log->setText("用户登录");

    }
    else if(logging::uservalue==3)
    {
        ui->log->setText("注销用户");
        ui->limitLabel->setText("用户权限");
        ui->levelLabel->setText("管理员");

    }
    else if(logging::uservalue==2)
    {
        ui->log->setText("注销用户");
        ui->limitLabel->setText("用户权限");
        ui->levelLabel->setText("可操作");


    }
    else if(logging::uservalue==1)
    {
        ui->log->setText("注销用户");
        ui->limitLabel->setText("用户权限");
        ui->levelLabel->setText("仅查看");
    }
}
void MainWindow::time()
{
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->time->setText(time);
//    usercheck();
}


//界面控制tcp
void MainWindow::interfaceControl()
{
    //显示部分
    QPixmap *pix0=new QPixmap(":/new/prefix1/YCshow.png");//pix0为筒内环境
    pix0->scaled(ui->showpix0->size(),Qt::KeepAspectRatio);
    QPixmap *pix1=new QPixmap(":/new/prefix1/location.png");//pix1为定位小图标
    pix1->scaled(ui->showpix0->size(),Qt::KeepAspectRatio);
    ui->showpix0->setScaledContents(true);
    ui->showpix0->setPixmap(*pix0);
    ui->showpix0_2->setScaledContents(true);
    ui->showpix0_2->setPixmap(*pix0);
    ui->showpix0_3->setScaledContents(true);
    ui->showpix0_3->setPixmap(*pix0);
    ui->loctionpic->setScaledContents(true);
    ui->loctionpic->setPixmap(*pix1);
    ui->loctionpic_2->setScaledContents(true);
    ui->loctionpic_2->setPixmap(*pix1);
    ui->loctionpic_3->setScaledContents(true);
    ui->loctionpic_3->setPixmap(*pix1);
    ui->loctionpic_4->setScaledContents(true);
    ui->loctionpic_4->setPixmap(*pix1);
    ui->loctionpic_5->setScaledContents(true);
    ui->loctionpic_5->setPixmap(*pix1);
    ui->loctionpic_6->setScaledContents(true);
    ui->loctionpic_6->setPixmap(*pix1);
    ui->loctionpic_7->setScaledContents(true);
    ui->loctionpic_7->setPixmap(*pix1);
    ui->loctionpic_8->setScaledContents(true);
    ui->loctionpic_8->setPixmap(*pix1);
    ui->loctionpic_9->setScaledContents(true);
    ui->loctionpic_9->setPixmap(*pix1);
    ui->loctionpic_10->setScaledContents(true);
    ui->loctionpic_10->setPixmap(*pix1);
    ui->loctionpic_11->setScaledContents(true);
    ui->loctionpic_11->setPixmap(*pix1);
    ui->loctionpic_12->setScaledContents(true);
    ui->loctionpic_12->setPixmap(*pix1);
    ui->loctionpic_13->setScaledContents(true);
    ui->loctionpic_13->setPixmap(*pix1);
    ui->loctionpic_14->setScaledContents(true);
    ui->loctionpic_14->setPixmap(*pix1);
    ui->loctionpic_15->setScaledContents(true);
    ui->loctionpic_15->setPixmap(*pix1);
    ui->loctionpic_16->setScaledContents(true);
    ui->loctionpic_16->setPixmap(*pix1);
    ui->loctionpic_17->setScaledContents(true);
    ui->loctionpic_17->setPixmap(*pix1);
    ui->loctionpic_18->setScaledContents(true);
    ui->loctionpic_18->setPixmap(*pix1);
    connect(ui->pushButton_1,&QPushButton::clicked,this,&MainWindow::widgetSwich1);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::widgetSwich2);
    connect(ui->pushButton_10,&QPushButton::clicked,this,&MainWindow::widgetSwich3);
}
void MainWindow::widgetSwich1()
{
    int index=0;
    ui->stackedWidget->setCurrentIndex(index);
}
void MainWindow::widgetSwich2()
{
    int index=1;
    ui->stackedWidget->setCurrentIndex(index);
}
void MainWindow::widgetSwich3()
{
    int index=2;
    ui->stackedWidget->setCurrentIndex(index);
}


//UDP通讯
void MainWindow::udp()
{
//    udpSender=new QUdpSocket();
//    ui->udpport->setText("8088");


    udpReceiver=new QUdpSocket();
    udpReceiver->bind(QHostAddress("127.0.0.1"),8088);
    connect(udpReceiver,SIGNAL(readyRead()),this,SLOT(udpReceive()));
    connect(this,SIGNAL(transfer(QByteArray)),this,SLOT(udpSenddata(QByteArray)));
}
//UDP接受指令信息
void MainWindow::udpReceive()
{
    QByteArray data;
    while(udpReceiver->hasPendingDatagrams())
    {
        data.resize(udpReceiver->pendingDatagramSize());
        udpReceiver->readDatagram(data.data(),data.size());
//        qDebug()<<"udp 接收"<<data;
        dataAnlysis(data);
    }
}

void MainWindow::udpSenddata(QByteArray data)
{
    udpReceiver->writeDatagram(data,QHostAddress("127.0.0.2"),8088);
    qDebug()<<"udp 发送";
}

void MainWindow::tcpSenddata(QByteArray data)
{
    tcpClient->write(data);
}

void MainWindow::tcpSenddata(QString data)
{
    tcpClient->write(data.toLatin1());
}



//套接字通讯
void MainWindow::tcp()
{
    tcpClient=new QTcpSocket(this);
    tcpClient->abort();
    connect(tcpClient,&QTcpSocket::readyRead,this,&MainWindow::readdataTcp);
    connect(tcpClient,&QTcpSocket::disconnected,this,&MainWindow::socketDiscon);//QT5
    connect(ui->send,&QPushButton::clicked,this,&MainWindow::senddata);
    connect(this,SIGNAL(transfer(QByteArray)),this,SLOT(tcpSenddata(QByteArray)));
    connect(this,SIGNAL(transfer(QString)),this,SLOT(tcpSenddata(QString)));
    ui->IP->setText("192.168.2.109");
    ui->port->setText("8080");
}
//TCP数据接收
void MainWindow::readdataTcp()
{
    QByteArray buffer;
    buffer=tcpClient->readAll();
    qDebug()<<"read data:"<<buffer;//暂未解析数据
    dataAnlysis(buffer);
    if(!buffer.isEmpty())
    {
        QString read=ui->textSend->toPlainText();
        read+=tr(buffer);
        ui->textSend->setText(read);
        qDebug()<<"已接收";
    }
}
void MainWindow::readerrorTcp()
{

}
void MainWindow::on_IPconnect_clicked()
{
    if(ui->IPconnect->text()=="开启监听")
    {
        QString IP;
        int port;
        IP=ui->IP->text();
        port=ui->port->text().toInt();
        tcpClient->abort();//取消已有的连接
        tcpClient->connectToHost(IP,port);//连接服务器
        if(!tcpClient->waitForConnected(30000))//等待连接成功
        {
            qDebug("连接失败");
            meBox->information(this,"通知","连接失败");
            return;
        }
        qDebug("连接成功");
        //void enabled() //开启按键使能
        ui->IPconnect->setText("断开连接");

    }
    else
    {
        tcpClient->disconnectFromHost();//断开连接
        ui->IPconnect->setText("开启监听");
        //void disenabled() //关闭按键使能

    }
}
void MainWindow::socketDiscon()
{
    //void disenabled() //关闭按键使能
    ui->IPconnect->setText("开启监听");
    meBox->information(this,"通知","连接断开");

}
void MainWindow::senddata()
{
    QString data;
    data=ui->textSend->toPlainText();
    tcpClient->write(data.toLatin1());
    qDebug()<<"已发送";
}


//数据解析
uint16_t MainWindow::modbusCRC(QByteArray senddata)//CRC校验
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
uint8_t station;//记录此时解析数据的地址
void MainWindow::writeData(int i, float data)//将解析后的数据显示在界面上
{
    QString datashow;
    if(station==0x0B)
    {
        datashow=QString("%1").arg(data,0,'g',4);
        switch (i) {
        case 1:ui->vol1_1->setText(datashow);break;
        case 2:ui->vol1_2->setText(datashow);break;
        case 3:ui->vol1_3->setText(datashow);break;
        case 4:ui->vol1_4->setText(datashow);break;
        case 5:ui->vol1_5->setText(datashow);break;
        case 6:ui->vol1_6->setText(datashow);break;
        case 7:ui->vol1_7->setText(datashow);break;
        case 8:ui->vol1_8->setText(datashow);break;
        case 9:ui->temperature1_1->setText(datashow);ui->temshow1_1->setText(datashow); break;
        case 10:ui->temperature1_2->setText(datashow);ui->temshow1_2->setText(datashow);break;
        case 11:ui->temperature1_3->setText(datashow);ui->temshow1_3->setText(datashow);break;
        case 12:ui->temperature1_4->setText(datashow);ui->temshow1_4->setText(datashow);break;
        case 13:ui->temperature1_5->setText(datashow);ui->temshow1_5->setText(datashow);break;
        case 14:ui->temperature1_6->setText(datashow);ui->temshow1_6->setText(datashow);break;
        }
    }
    else if(station==0x0C)
    {
        datashow=QString("%1").arg(data,0,'g',4);
        switch (i) {
        case 1:ui->vol2_1->setText(datashow);break;
        case 2:ui->vol2_2->setText(datashow);break;
        case 3:ui->vol2_3->setText(datashow);break;
        case 4:ui->vol2_4->setText(datashow);break;
        case 5:ui->vol2_5->setText(datashow);break;
        case 6:ui->vol2_6->setText(datashow);break;
        case 7:ui->vol2_7->setText(datashow);break;
        case 8:ui->vol2_8->setText(datashow);break;
        case 9:ui->temperature2_1->setText(datashow);ui->temshow2_1->setText(datashow);break;
        case 10:ui->temperature2_2->setText(datashow);ui->temshow2_2->setText(datashow);break;
        case 11:ui->temperature2_3->setText(datashow);ui->temshow2_3->setText(datashow);break;
        case 12:ui->temperature2_4->setText(datashow);ui->temshow2_4->setText(datashow);break;
        case 13:ui->temperature2_5->setText(datashow);ui->temshow2_5->setText(datashow);break;
        case 14:ui->temperature2_6->setText(datashow);ui->temshow2_6->setText(datashow);break;
        }
    }
    else if(station==0x0D)
    {
        datashow=QString("%1").arg(data,0,'g',4);
        switch (i) {
        case 1:ui->vol3_1->setText(datashow);break;
        case 2:ui->vol3_2->setText(datashow);break;
        case 3:ui->vol3_3->setText(datashow);break;
        case 4:ui->vol3_4->setText(datashow);break;
        case 5:ui->vol3_5->setText(datashow);break;
        case 6:ui->vol3_6->setText(datashow);break;
        case 7:ui->vol3_7->setText(datashow);break;
        case 8:ui->vol3_8->setText(datashow);break;
        case 9:ui->temperature3_1->setText(datashow);ui->temshow3_1->setText(datashow);break;
        case 10:ui->temperature3_2->setText(datashow);ui->temshow3_2->setText(datashow);break;
        case 11:ui->temperature3_3->setText(datashow);ui->temshow3_3->setText(datashow);break;
        case 12:ui->temperature3_4->setText(datashow);ui->temshow3_4->setText(datashow);break;
        case 13:ui->temperature3_5->setText(datashow);ui->temshow3_5->setText(datashow);break;
        case 14:ui->temperature3_6->setText(datashow);ui->temshow3_6->setText(datashow);break;
        }
    }
    else if(station==0x0E)
    {
        datashow=QString("%1").arg(data,0,'g',4);
        switch (i) {
        case 0:ui->ZH1_0->setText(datashow);break;
        case 3:ui->ZH1_3 ->setText(datashow);break;
        case 6:ui->ZH1_6 ->setText(datashow);break;
        case 24:ui->ZH1_24 ->setText(datashow);break;
        case 27:ui->ZH1_27 ->setText(datashow);break;
        case 46:ui->ZH1_46 ->setText(datashow);break;
        }
    }
}
//分析传来的数据包并根据地址使用不同的函数
void MainWindow::dataAnlysis(QByteArray buffer)//数据解析
{
    station=buffer[0];
    qDebug()<<"station="<<station;
    if(station==0x0F||station==0x03||station==0x0B||station==0x0C||station==0x0D)
    {
//        qDebug()<<"YC数据";
        YCdata(buffer);
    }
    else if(station==0x0E)
    {
//        qDebug()<<"ZH数据";
        ZHdata(buffer);
    }
    else
    {
        qDebug()<<"测试数据"<<buffer.toHex();
    }
}
void MainWindow::YCdata(QByteArray buffer)//YC数据解析 IEEE754浮点数解析
{
    qDebug()<<"YCbuffer:"<<buffer.toHex();
    for(int i=3,j=1;i<buffer.length()-5;i+=4,j++)
    {

        uint64_t b23=0;b23+=pow(2,23);
        uint64_t b31=0;b31+=pow(2,31);
        uint32_t temp=0;
        uint8_t temp2=0,temp3=0,temp4=0,temp5=0;
        float temp1;
        float data;
        uint64_t a1=0,a2=0,a3=0;
        temp2+=buffer.data()[i];
        temp3+=buffer.data()[i+1];
        temp4+=buffer.data()[i+2];
        temp5+=buffer.data()[i+3];
        //qDebug()<<"111111:"<<buffer.toHex()<<endl;
//        temp=(buffer[i]+1)*pow(2,24)+buffer[i+1]*pow(2,16)+buffer[i+2]*64+buffer[i+3];
        temp=temp2*pow(2,24)+temp3*pow(2,16)+temp4*pow(2,8)+temp5;
//        qDebug()<<"temp2"<<hex<<temp2;
//        qDebug()<<"temp3"<<hex<<temp3;
//        qDebug()<<"temp4"<<hex<<temp4;
//        qDebug()<<"temp5"<<hex<<temp5;
//        qDebug()<<"temp="<<hex<<temp;
//        qDebug()<<"temp="<<bin<<temp;
        a3=temp%b23;
//        qDebug()<<"a3="<<hex<<a3;
        a2=(temp%b31-a3)/b23;
        a1=(temp-a2-a3)/b31;
//        qDebug()<<"a1:"<<a1;
//        qDebug()<<"a2:"<<a2;
//        qDebug()<<"a3:"<<a3;
        temp1=a3*pow(2,-23);//2(-23)
//        qDebug()<<"temp1:"<<temp1;
        data=pow(-1,a1)*pow(2,a2-127)*(1+temp1);
//        qDebug()<<"data="<<data;
        writeData(j,data);
    }
}
uint16_t curRange;
uint16_t volRange;
void MainWindow::ZHdata(QByteArray buffer)//ZH数据解析
{
    curRange=ui->curTop->text().toInt();
    volRange=ui->volTop->text().toInt();
    //每个寄存器两字节数据，数据区前三字节，第二字节为数据区长度
    qDebug()<<"ZHdata"<<buffer.toHex();
    uint32_t temp;
    uint8_t temp1=0,temp2=0;
//    temp1+=buffer.data()[3];
//    temp2+=buffer.data()[3+1];
//    temp=temp1*pow(2,8)+temp2;
//    qDebug()<<"vol ZHdata:"<<temp;
//    temp1=buffer.data()[5];
//    temp2=buffer.data()[5+1];
//    temp=temp1*pow(2,8)+temp2;
//    qDebug()<<"cur ZHdata:"<<temp;
    uint16_t data;
    int function=buffer[1];
    if(function==0x03)
    {
        for(int i=3,j=0;i<buffer.length()-2;i+=2,j++)
        {
            temp1=buffer.data()[i];
            temp2=buffer.data()[i+1];
            temp=temp1*pow(2,8)+temp2;
            switch (j) {
            case 0:data=temp/10000*volRange;writeData(j,data);break;//A相电压
            case 1:data=temp/10000*volRange;break;//B相电压
            case 2:data=temp/10000*volRange;break;//C相电压
            case 3:data=temp/10000*curRange;writeData(j,data);break;//A相电流
            case 4:data=temp/10000*curRange;break;//B相电流
            case 5:data=temp/10000*curRange;break;//C相电流
            case 6:data=temp/10000*curRange*volRange;writeData(j,data);break;//A相有功功率
            case 7:data=temp/10000*curRange*volRange;break;//B相有功功率
            case 8:data=temp/10000*curRange*volRange;break;//C相有功功率
            case 9:data=temp/10000;writeData(j,data);break;//A相功率因数
            case 10:data=temp/10000;break;//B相功率因数
            case 11:data=temp/10000;break;//C相功率因数
            case 12:data=temp/10000*3*curRange*volRange;break;//总有功功率(三相三线时不*3)
            case 13:data=temp/10000*3*curRange*volRange;break;//总无功功率(三相三线时不*3)
            case 14:data=temp/10000;break;//总功率因数
            case 15:data=temp/100;break;//频率
            case 24:data=temp/10000*curRange*volRange;writeData(j,data);break;//A相无功功率
            case 25:data=temp/10000*curRange*volRange;break;//B相无功功率
            case 26:data=temp/10000*curRange*volRange;break;//C相无功功率
            case 27:data=temp/10000*curRange*volRange;writeData(j,data);break;//A相现在功率
            case 28:data=temp/10000*curRange*volRange;break;//B相现在功率
            case 29:data=temp/10000*curRange*volRange;break;//C相现在功率
            case 30:data=temp/10000*curRange*volRange;break;//总现在功率
            case 31:data=temp/10000*volRange;break;//AB相线电压
            case 32:data=temp/10000*volRange;break;//BC相线电压
            case 33:data=temp/10000*volRange;break;//CA相线电压
            case 41:data=temp/10000*curRange*volRange;break;//B相
            case 46:data=temp/100;writeData(j,data);break;//频率
            }
        }//i-4 i-3 i-2 i-1
    }
    else if(function==0x10)
    {
        //弹窗设置成功
        meBox->information(this,"通知","ZH设置成功");
    }
}
//电柜控制，串口通讯
QByteArray electricData;
bool enableElecsend=true;
void MainWindow::electricBox()
{
    serial=new QSerialPort();
    serial->setPortName("待定");
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    connect(serial,&QSerialPort::readyRead,this,&MainWindow::readElectric);
    if(serial->isOpen())
    {
        ui->elecConnect->setText("串口已连接");
        ui->elecChange->setEnabled(true);
        ui->elecEnable->setEnabled(true);

    }
    connect(timer,&QTimer::timeout,this,&MainWindow::timeupdateElec);
}
//等待时间接受完整电柜串口信息
void MainWindow::timeupdateElec()
{
    qDebug()<<"Elec read data:";
    timer->stop();
    enableElecsend=true;
    if(!electricData.isEmpty())
    {
        qDebug()<<electricData;//处理电柜回传数据部分
    }
    electricData.clear();   
}
//读取电柜传来的信息
void MainWindow::readElectric()
{
    timer->start(820);
    electricData.append(serial->readAll());

}

//电柜设置参数按钮槽函数
void MainWindow::on_elecChange_clicked()
{

    float frequency=ui->lineEdit->text().toFloat();//频率
    int voltage=ui->lineEdit_35->text().toInt();//电压
    float currentLimit=ui->lineEdit_36->text().toFloat();//电流上限
    uint16_t fre=frequency*10,vol=voltage,cur=currentLimit*100;
    QByteArray senddata;
    uint16_t crc;
    senddata.resize(8);

    if(enableElecsend){
        senddata[0]=0x64;//地址
        senddata[1]=0x06;senddata[2]=0x00;senddata[3]=0x0C;senddata[5]=fre%256;senddata[4]=(fre-fre%256)/256;
        crc=modbusCRC(senddata);
        senddata[7]=crc%256;senddata[6]=(crc-crc%256)/256;
        qDebug()<<"senddata fre:"<<senddata.toHex();
        serial->write(senddata);
        enableElecsend=false;
        serial->waitForReadyRead(1000);

    }

    if(enableElecsend){
        if(senddata==electricData)  ui->label_49->setText("设置频率成功");
        else ui->label_49->setText("设置频率失败");
        senddata[3]=0x0D;senddata[5]=vol%256;senddata[4]=(vol-vol%256)/256;
        crc=modbusCRC(senddata);
        senddata[7]=crc%256;senddata[6]=(crc-crc%256)/256;
        qDebug()<<"senddata vol:"<<senddata.toHex();
        serial->write(senddata);
        enableElecsend=false;
        serial->waitForReadyRead(1000);
    }

    if(enableElecsend){
        if(senddata==electricData)  ui->label_50->setText("设置电压成功");
        else ui->label_50->setText("设置电压失败");
        senddata[3]=0x0F;senddata[5]=cur%256;senddata[4]=(cur-cur%256)/256;
        crc=modbusCRC(senddata);
        senddata[7]=crc%256;senddata[6]=(crc-crc%256)/256;
        qDebug()<<"senddata cur:"<<senddata.toHex();
        serial->write(senddata);
        enableElecsend=false;
        serial->waitForReadyRead(1000);
    }
    if(enableElecsend){
        if(senddata==electricData)  ui->label_54->setText("设置电流上限成功");
        else ui->label_54->setText("设置电流上限失败");
    }
//    qDebug()<<hex<<cur;
//    qDebug()<<hex<<cur%256;
//    qDebug()<<hex<<(cur-cur%256);
//    qDebug()<<senddata[5];
//    qDebug()<<senddata[6];
}
//电柜控制按钮槽函数
void MainWindow::on_elecEnable_clicked()
{
    QByteArray senddata;
    senddata.resize(8);
    if(ui->elecEnable->text()=="启动电源")
    {
        senddata[0]=0x64;//地址
        senddata[1]=0x06;senddata[2]=0x00;senddata[3]=0x0E;senddata[5]=0x01;senddata[4]=0x00;
        uint16_t crc=modbusCRC(senddata);
        senddata[7]=crc%256;senddata[6]=(crc-crc%256)/256;
        qDebug()<<"senddata elecEnable:"<<senddata.toHex();
        serial->write(senddata);
        enableElecsend=false;
        serial->waitForReadyRead(1000);
        if(enableElecsend){
            if(senddata==electricData)  ui->elecEnable->setText("关闭电源");
        }
    }
    else if(ui->elecEnable->text()=="关闭电源")
    {
        senddata[0]=0x64;//地址
        senddata[1]=0x06;senddata[2]=0x00;senddata[3]=0x0E;senddata[5]=0x00;senddata[4]=0x00;
        uint16_t crc=modbusCRC(senddata);
        senddata[7]=crc%256;senddata[6]=(crc-crc%256)/256;
        qDebug()<<"senddata elecEnable:"<<senddata.toHex();
        serial->write(senddata);
        enableElecsend=false;
        serial->waitForReadyRead(1000);
        if(enableElecsend){
            if(senddata==electricData)  ui->elecEnable->setText("启动电源");
        }
    }
}
//ZH电流电压设置按钮槽函数
void MainWindow::on_setVolTop_clicked()
{
    QByteArray sendData;
    sendData.resize(13);
    sendData[0]=0x0E;sendData[1]=0x10;sendData[2]=0x00;sendData[3]=0x53;sendData[4]=0x00;sendData[5]=0x02;sendData[6]=0x04;
    uint16_t vol=ui->volTop->text().toInt();
    uint16_t cur=ui->curTop->text().toInt();
    sendData[8]=vol%256;sendData[7]=(vol-vol%256)/256;sendData[10]=cur%256;sendData[9]=(cur-cur%256)/256;//写入电压电流量程
    uint16_t CRC=modbusCRC(sendData);
    uint16_t crcF=CRC%256;
    uint16_t crcB=(CRC-crcF)/256;
    sendData[11]=crcF;sendData[12]=crcB;
    transfer(sendData);
}
//
//2021/4/15:数据解析，将QBytearray存储到变量中时变量空间太大会导致高位为1使数据错误。（已解决）
//2021/4/16:笔记：TCP传输是基于流的传输，不保护消息边界，多个数据包看作一个数据流，多个数据包发送间隔短会产生粘包问题，接收端只需要一次接受动作就可以接受多个数据包；
//               UDP传输是保护消息边界的，发送多个数据包接收端需要进行多次接收动作才能完成接收，所以UDP不会产生粘包问题。
//          解决方法：(1)发送固定长度的消息(2)把消息的尺寸与消息一块发送(3)使用特殊标记来区分消息间隔
//2021/5/8:问题：ZH读数据太长会和YC数据粘接在一起，解决长数据独立问题。测试结果1：串口数据接收有问题。（传过去的数据难以插入到消息队列中，需要添加缓冲区）                                                                                                消息重合问题

void MainWindow::on_fanElec_open_clicked()
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x06;sendData[3]=0x0B;sendData[4]=0x01;sendData[5]=0x35;sendData[6]=0x00;
    sendData[7]=0xCD;sendData[8]=0x4D;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}

void MainWindow::on_temElec_open_clicked()
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x06;sendData[3]=0x0B;sendData[4]=0x02;sendData[5]=0x35;sendData[6]=0x00;
    sendData[7]=0x3D;sendData[8]=0x4D;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}

void MainWindow::on_pushButton_17_clicked()
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x06;sendData[3]=0x0B;sendData[4]=0x03;sendData[5]=0x35;sendData[6]=0x00;
    sendData[7]=0x6C;sendData[8]=0x8D;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}

void MainWindow::on_pushButton_19_clicked()
{
    QByteArray sendData;
    sendData.resize(10);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x09;sendData[3]=0x0A;sendData[4]=0x09;sendData[5]=0xEE;sendData[6]=0xF8;
    sendData[7]=0x42;sendData[8]=0x0D;sendData[9]=0x0A;
    transfer(sendData);
}

void MainWindow::on_pushButton_20_clicked()
{
    QByteArray sendData;
    sendData.resize(10);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x0B;sendData[3]=0x0A;sendData[4]=0x09;sendData[5]=0xEE;sendData[6]=0xF9;
    sendData[7]=0xFA;sendData[8]=0x0D;sendData[9]=0x0A;
    transfer(sendData);
}

void MainWindow::on_temElec_close_clicked()
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x06;sendData[3]=0x0B;sendData[4]=0x02;sendData[5]=0x3A;sendData[6]=0x00;
    sendData[7]=0x38;sendData[8]=0xBD;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}

void MainWindow::on_fanElec_close_clicked()
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x06;sendData[3]=0x0B;sendData[4]=0x01;sendData[5]=0x3A;sendData[6]=0x00;
    sendData[7]=0xC8;sendData[8]=0xBD;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}

void MainWindow::on_waveElec_close_clicked()
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x3A;sendData[1]=0x02;sendData[2]=0x06;sendData[3]=0x0B;sendData[4]=0x03;sendData[5]=0x3A;sendData[6]=0x00;
    sendData[7]=0x69;sendData[8]=0x7D;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}

void MainWindow::on_pushButton_clicked()//定时器使能
{
    QByteArray sendData;
    sendData.resize(11);
    sendData[0]=0x24;sendData[1]=0x54;sendData[2]=0x52;sendData[3]=0x47;sendData[4]=0x2C;sendData[5]=0x33;sendData[6]=0x36;
    sendData[7]=0x2C;sendData[8]=0x2A;sendData[9]=0x0D;sendData[10]=0x0A;
    transfer(sendData);
}


void MainWindow::on_pushButton_11_clicked()//波形发送
{
    QByteArray sendData;
    if(ui->waveFrequency->currentText()=="0.01Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4D; sendData[4] = 0x2C; sendData[5] = 0x02;
        sendData[6] = 0x68; sendData[7] = 0x4D; sendData[8] = 0xF3; sendData[9] = 0x9C; sendData[10] = 0x68; sendData[11] = 0x4D;
        sendData[12] = 0xF3; sendData[13] = 0x9C; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="0.1Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4D; sendData[4] = 0x2C; sendData[5] = 0x02;
        sendData[6] = 0x0A; sendData[7] = 0x6E; sendData[8] = 0x31; sendData[9] = 0xF6; sendData[10] = 0x0A; sendData[11] = 0x6E;
        sendData[12] = 0x31; sendData[13] = 0xF6; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="0.25Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x04; sendData[7] = 0x2C; sendData[8] = 0x13; sendData[9] = 0xFC; sendData[10] = 0x04; sendData[11] = 0x2C;
        sendData[12] = 0x13; sendData[13] = 0xFC; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="1Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x01; sendData[7] = 0x0B; sendData[8] = 0x04; sendData[9] = 0xFF; sendData[10] = 0x01; sendData[11] = 0x0B;
        sendData[12] = 0x04; sendData[13] = 0xFF; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="5Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x35; sendData[8] = 0x67; sendData[9] = 0x66; sendData[10] = 0x00; sendData[11] = 0x35;
        sendData[12] = 0x67; sendData[13] = 0x66; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="10Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x1a; sendData[8] = 0xB3; sendData[9] = 0xB3; sendData[10] = 0x00; sendData[11] = 0x1a;
        sendData[12] = 0xB3; sendData[13] = 0xB3; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="50Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x05; sendData[8] = 0x57; sendData[9] = 0x24; sendData[10] = 0x00; sendData[11] = 0x05;
        sendData[12] = 0x57; sendData[13] = 0x24; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="2Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x85; sendData[8] = 0x82; sendData[9] = 0x80; sendData[10] = 0x00; sendData[11] = 0x85;
        sendData[12] = 0x82; sendData[13] = 0x80; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="0.5Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x02; sendData[7] = 0x16; sendData[8] = 0x09; sendData[9] = 0xFE; sendData[10] = 0x02; sendData[11] = 0x16;
        sendData[12] = 0x09; sendData[13] = 0xFE; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="15Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x11; sendData[8] = 0xcD; sendData[9] = 0x22; sendData[10] = 0x00; sendData[11] = 0x11;
        sendData[12] = 0xcD; sendData[13] = 0x22; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="0.05Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x14; sendData[7] = 0xdC; sendData[8] = 0x63; sendData[9] = 0xEC; sendData[10] = 0x14; sendData[11] = 0xdC;
        sendData[12] = 0x63; sendData[13] = 0xEC; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="20Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x0d; sendData[8] = 0x59; sendData[9] = 0xD9; sendData[10] = 0x00; sendData[11] = 0x0d;
        sendData[12] = 0x59; sendData[13] = 0xD9; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="25Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x0a; sendData[8] = 0xaE; sendData[9] = 0x48; sendData[10] = 0x00; sendData[11] = 0x0a;
        sendData[12] = 0xaE; sendData[13] = 0x48; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="30Hz")
    {
        sendData.resize(16);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x02;
        sendData[6] = 0x00; sendData[7] = 0x08; sendData[8] = 0xe6; sendData[9] = 0x91; sendData[10] = 0x00; sendData[11] = 0x08;
        sendData[12] = 0xe6; sendData[13] = 0x91; sendData[14] = 0x0D; sendData[15] = 0x0A;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="0.1Hz+")
    {
        sendData.resize(48);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x0a;
        sendData[6] = 0x01; sendData[7] = 0x0b; sendData[8] = 0x04; sendData[9] = 0xFF;
        sendData[10] = 0x01; sendData[11] = 0x0b; sendData[12] = 0x04; sendData[13] = 0xFF;
        sendData[14] = 0x02; sendData[15] = 0x16; sendData[16] = 0x09; sendData[17] = 0xfe;
        sendData[18] = 0x04; sendData[19] = 0x2c; sendData[20] = 0x13; sendData[21] = 0xfc;
        sendData[22] = 0x02; sendData[23] = 0x16; sendData[24] = 0x09; sendData[25] = 0xfe;
        sendData[26] = 0x01; sendData[27] = 0x0b; sendData[28] = 0x04; sendData[29] = 0xff;
        sendData[30] = 0x01; sendData[31] = 0x0b; sendData[32] = 0x04; sendData[33] = 0xff;
        sendData[34] = 0x02; sendData[35] = 0x16; sendData[36] = 0x09; sendData[37] = 0xfe;
        sendData[38] = 0x04; sendData[39] = 0x2c; sendData[40] = 0x13; sendData[41] = 0xfc;
        sendData[42] = 0x02; sendData[43] = 0x16; sendData[44] = 0x09; sendData[45] = 0xfe;
        sendData[46] = 0x0d; sendData[47] = 0x0a;
        transfer(sendData);
    }
    else if(ui->waveFrequency->currentText()=="0.25Hz+")
    {
        sendData.resize(48);
        sendData[0] = 0x24; sendData[1] = 0x50; sendData[2] = 0x57; sendData[3] = 0x4d; sendData[4] = 0x2c; sendData[5] = 0x0a;
        sendData[6] = 0x00; sendData[7] = 0x6a; sendData[8] = 0xce; sendData[9] = 0xcc;
        sendData[10] = 0x00; sendData[11] = 0x6a; sendData[12] = 0xce; sendData[13] = 0xcc;
        sendData[14] = 0x00; sendData[15] = 0xd5; sendData[16] = 0x9d; sendData[17] = 0x98;
        sendData[18] = 0x01; sendData[19] = 0xab; sendData[20] = 0x3b; sendData[21] = 0x30;
        sendData[22] = 0x00; sendData[23] = 0xd5; sendData[24] = 0x9d; sendData[25] = 0x98;
        sendData[26] = 0x00; sendData[27] = 0x6a; sendData[28] = 0xce; sendData[29] = 0xcc;
        sendData[30] = 0x00; sendData[31] = 0x6a; sendData[32] = 0xce; sendData[33] = 0xcc;
        sendData[34] = 0x00; sendData[35] = 0xd5; sendData[36] = 0x9d; sendData[37] = 0x98;
        sendData[38] = 0x01; sendData[39] = 0xab; sendData[40] = 0x3b; sendData[41] = 0x30;
        sendData[42] = 0x00; sendData[43] = 0xd5; sendData[44] = 0x9d; sendData[45] = 0x98;
        sendData[46] = 0x0d; sendData[47] = 0x0a;
        transfer(sendData);
    }

}


void MainWindow::on_pushButton_12_clicked()//硬件使能
{
    QByteArray sendData;
    sendData.resize(12);
    sendData[0]=0x24;sendData[1]=0x45;sendData[2]=0x4E;sendData[3]=0x44;sendData[4]=0x49;sendData[5]=0x53;sendData[6]=0x2C;
    sendData[7]=0x31;sendData[8]=0x2C;sendData[9]=0x2A;sendData[10]=0x0D;sendData[11]=0x0A;
    transfer(sendData);
}

void MainWindow::on_pushButton_13_clicked()//软件使能
{
    QByteArray sendData;
    sendData.resize(10);
    sendData[0] = 0x24; sendData[1] = 0x4F; sendData[2] = 0x55; sendData[3] = 0x54; sendData[4] = 0x2c; sendData[5] = 0x31;
    sendData[6] = 0x2C; sendData[7] = 0x2A; sendData[8] = 0x0D; sendData[9] = 0x0A;
    transfer(sendData);
}

void MainWindow::on_Start7606_clicked()
{
    QString sendData;
    sendData="@$PON";
    transfer(sendData);
}

void MainWindow::on_Stop7606_clicked()
{
    QString sendData;
    sendData="@$POF";
    transfer(sendData);
}

void MainWindow::on_SDcardReading_clicked()
{
    QString sendData;
    sendData="@$RSD";
    transfer(sendData);
}

void MainWindow::on_SDmemory_clicked()
{
    QString sendData;
    sendData="@$STA";
    transfer(sendData);
}
