#ifndef LOGGING_H
#define LOGGING_H
#include <QWidget>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include"registe.h"

namespace Ui {
class logging;
}

class logging : public QWidget
{
    Q_OBJECT

public:
    explicit logging(QWidget *parent = nullptr);
    registe regist;
    static int uservalue;
    static QString userNow;
    void userDatabase();
    ~logging();
signals:
    void userlog();
private slots:
    void on_pushButton_clicked();
    void on_log_clicked();

    void on_selectusers_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_deleteUser_clicked();

private:
    Ui::logging *ui;
    QSqlDatabase userData;
    QSqlQuery *sql;
    QSqlQueryModel *model;
};

#endif // LOGGING_H
