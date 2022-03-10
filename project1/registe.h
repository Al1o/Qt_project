#ifndef REGISTE_H
#define REGISTE_H
#include <QWidget>
#include <QSqlDatabase>
#include<QSqlQuery>
#include<QString>
#include<QMessageBox>
#include<QDebug>
namespace Ui {
class registe;
}

class registe : public QWidget
{
    Q_OBJECT

public:
    explicit registe(QWidget *parent = nullptr);
    void userDatabase();
    ~registe();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::registe *ui;
    QSqlDatabase userData;
    QSqlQuery *sql;
};

#endif // REGISTE_H
