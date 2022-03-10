#include "mythread5.h"
int port=60000;
mythread5::mythread5(QObject *parent) : QThread(parent)
{

}

void mythread5::newTcpconnect()
{
    qDebug()<<"已连接7606qian";
    socket2=server2->nextPendingConnection();//与客户端连接
    connect(socket2,&QTcpSocket::readyRead,this,&mythread5::tcpReadData);
    connect(socket2,&QTcpSocket::disconnected,this,&mythread5::socketDiscon);
    qDebug()<<"已连接7606";
    emit done();
}

void mythread5::tcpReadData()
{
    QByteArray buffer;
    buffer=socket2->readAll();

    if(!buffer.isEmpty())
    {
        qDebug()<<buffer;
        emit transfer(buffer);
    }
}

void mythread5::tcpSendData()
{

    if(socket2->isOpen()){
    socket2->write(bufferSendQueue.dequeue());
    socket2->flush();
    socket2->waitForBytesWritten();
    }
    if(!bufferSendQueue.isEmpty())
    {
        tcpSendData();
    }

}

void mythread5::socketDiscon()
{
    qDebug()<<"客户端连接已断开";
}

void mythread5::run()
{
    qDebug()<<"t5 run";
    Tcpconnect();
    exec();
}

void mythread5::Tcpconnect()
{
    server2=new QTcpServer();
//    socket2=new QTcpSocket();
//    connect(server2,&QTcpserver2::newConnection,this,&mythread5::newTcpconnect);
    connect(server2,&QTcpServer::newConnection,this,&mythread5::newTcpconnect);
//    connect(server2,SIGNAL(newConnection()),this,SLOT(newTcpconnect()));
    qDebug()<<"111";
    if(!server2->listen(QHostAddress::Any,port))
    {
        qDebug()<<server2->errorString();
        return;
    }
    qDebug()<<"侦听成功";

}

void mythread5::enSendQueue(QByteArray sendToPortdata)
{
    qDebug()<<"进入发送";
    bufferSendQueue.enqueue(sendToPortdata);
    tcpSendData();
}


