#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QtWidgets>
#include <QMainWindow>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QTcpSocket>
#include<QHostAddress>
#include<QMessageBox>
#include<QTime>
#include<QTimer>
#include<QString>
#include<QSqlDatabase>
#include<QSqlQueryModel>
#include<QLabel>
#include<QPixmap>
#include<typeinfo>
#include<QUdpSocket>
#include"logging.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void widgetSwich1();
    void widgetSwich2();
    void widgetSwich3();
    void interfaceControl();
    void electricBox();//电柜控制
    void tcp();
    void udp();
    void dataB();//数据库
    void timeupdateElec();
    void writeData(int i,float data);
    logging log;
    QPushButton *pushbutton_1=new QPushButton;
    //数据库
    QSqlDatabase database=QSqlDatabase::addDatabase("QMYSQL");

private slots:
    void readdataTcp();
    void readerrorTcp();
    void on_IPconnect_clicked();
    void on_log_clicked();
    void usercheck();
    void time();
    uint16_t modbusCRC(QByteArray senddata);
    void socketDiscon();
    void senddata();
    void dataAnlysis(QByteArray buffer);//数据解析
    void YCdata(QByteArray buffer);//YC温度模拟量读取
    void ZHdata(QByteArray buffer);
    void udpReceive();
    void udpSenddata(QByteArray);//UDP数据发送
    void tcpSenddata(QByteArray);//TCP数据发送
    void tcpSenddata(QString);
    void readElectric();//甲板电源
    void on_elecChange_clicked();
    void on_elecEnable_clicked();
    void on_setVolTop_clicked();
    void on_fanElec_open_clicked();
    
    void on_temElec_open_clicked();
    
    void on_pushButton_17_clicked();
    
    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_temElec_close_clicked();

    void on_fanElec_close_clicked();

    void on_waveElec_close_clicked();

    void on_pushButton_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_Start7606_clicked();

    void on_Stop7606_clicked();

    void on_SDcardReading_clicked();

    void on_SDmemory_clicked();

signals:
    void transfer(QByteArray);//数据传输
    void transfer(QString);
private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    QSerialPort *serial;
    QTcpSocket *tcpClient;
    QMessageBox *meBox;
    QTimer *timer=Q_NULLPTR;
    QSqlDatabase dataBase;
    QSqlQuery *sql;
//    QUdpSocket udpSender;
    QUdpSocket *udpReceiver;
    logging *loggingui=new logging();


};
#endif // MAINWINDOW_H
