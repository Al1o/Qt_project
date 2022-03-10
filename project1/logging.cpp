#include "logging.h"
#include "ui_logging.h"
#include"registe.h"
#include<QObject>
#include"mainwindow.h"
int logging::uservalue=0;
QString logging::userNow=NULL;
logging::logging(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logging)
{
    ui->setupUi(this);
    ui->deleteUser->hide();
    userDatabase();
    ui->password_2->setEchoMode(QLineEdit::Password);
}

void logging::userDatabase()
{
    userData=QSqlDatabase::addDatabase("QSQLITE");
    userData.setDatabaseName("userData.db");
    userData.open();
    sql=new QSqlQuery();
//    sql->exec("create table user('username','password','level')");
//    sql->exec("insert into user(username,password,level) values('ZGHD','000313','3')");
}
logging::~logging()
{
    delete ui;
}

void logging::on_pushButton_clicked()
{
    regist.show();
}
//判断登录
void logging::on_log_clicked()
{
    userData=QSqlDatabase::addDatabase("QSQLITE");
    userData.setDatabaseName("userData.db");
    userData.open();
    sql=new QSqlQuery();
    QString username=ui->user_2->text();
    QString password =ui->password_2->text();
    sql->exec("select * from user where username='"+username+"'");
    if(sql->next())
    {
        if(password==sql->value(1).toString())
        {
            logging::uservalue=sql->value(2).toInt();
            logging::userNow=sql->value(0).toString();
            ui->password_2->clear();
            this->close();
            userlog();
        }
        else if(password!=sql->value(1).toString())
        {
            ui->label->setText("密码输入错误！");
            qDebug()<<"密码输入错误";
            ui->password_2->clear();
        }
    }
    else
    {
        ui->label->setText("未有该用户！");
        qDebug()<<"未有该用户";
        ui->password_2->clear();
    }

}
//查询用户组
void logging::on_selectusers_clicked()
{
    userData=QSqlDatabase::addDatabase("QSQLITE");
    userData.setDatabaseName("userData.db");
    userData.open();
    sql=new QSqlQuery();
    model=new QSqlQueryModel();
    model->setQuery("select username as 用户名,password as 密码,level as 权限 from user");
    ui->tableView->setModel(model);
    ui->deleteUser->show();
}
//获取用户值
void logging::on_tableView_clicked(const QModelIndex &index)
{
    QString username=index.siblingAtColumn(0).data().toString( );
    ui->user_2->setText(username);
    QString password=index.siblingAtColumn(1).data().toString();
    ui->password_2->setText(password);
}
//删除用户
void logging::on_deleteUser_clicked()
{
    QString username=ui->user_2->text();
//    userData=QSqlDatabase::addDatabase("QSQLITE");
//    userData.setDatabaseName("userData.db");
//    userData.open();
    sql=new QSqlQuery();
    sql->exec("delete from user where username='"+username+"'");
    model->setQuery("select username as 用户名,password as 密码,level as 权限 from user");
}
