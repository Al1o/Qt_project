#include "registe.h"
#include "ui_registe.h"

registe::registe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registe)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    ui->adminPassword->setEchoMode(QLineEdit::Password);
    userDatabase();
}
//连接到数据库
void registe::userDatabase()
{
    userData=QSqlDatabase::addDatabase("QSQLITE");
    userData.setDatabaseName("userData.db");
    userData.open();
    sql=new QSqlQuery();
    sql->exec("create table user(username,password,level)");

}

registe::~registe()
{
    delete ui;
}
//确认管理员身份
void registe::on_pushButton_2_clicked()
{
    userData=QSqlDatabase::addDatabase("QSQLITE");
    userData.setDatabaseName("userData.db");
    userData.open();
    sql=new QSqlQuery();
    QString username=ui->adminUser->text();
    QString password =ui->adminPassword->text();
    sql->exec("select * from user where username='"+username+"'");
//    sql->next();
    if(sql->next())
    {
        if(password==sql->value(1).toString())
        {
            if(sql->value(2).toInt()==3)
            {
                ui->pushButton_2->setText("确认通过");
                ui->pushButton->setEnabled(true);
            }
            else
            {
                ui->pushButton_2->setText("该用户无权限");
                ui->adminPassword->clear();
                ui->adminUser->clear();
            }
        }
        else if(password!=sql->value(1).toString())
        {
            ui->pushButton_2->setText("请重新输入");
            ui->adminPassword->clear();
        }
    }
    else
    {
        ui->pushButton_2->setText("无该用户");
        ui->adminUser->clear();
        ui->adminPassword->clear();
    }
}
//注册新用户
void registe::on_pushButton_clicked()
{
//    userData=QSqlDatabase::addDatabase("QSQLITE");
//    userData.setDatabaseName("userData.db");
//    userData.open();
    sql=new QSqlQuery();
    QString username=ui->user_3->text();
    QString password=ui->password_3->text();
    if(ui->comboBox->currentIndex()==0)
    {
        sql->exec("insert into user(username,password,level) values('"+username+"','"+password+"',3)");
    }
    else if(ui->comboBox->currentIndex()==1)
    {
        sql->exec("insert into user(username,password,level) values('"+username+"','"+password+"',2)");
    }
    else
    {
        sql->exec("insert into user(username,password,level) values('"+username+"','"+password+"',1)");
    }
    QMessageBox::about(this,"消息","注册成功");
}
