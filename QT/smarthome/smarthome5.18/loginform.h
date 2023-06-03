#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include "modify_pass.h"
#include "creat_anc.h"
#include <QTcpSocket>
//#include "warning.h"
#include "error.h"

namespace Ui {
class loginForm;
}

class loginForm : public QWidget
{
    Q_OBJECT
signals: //我支持的信号
    void login_successed();
    //void login_signal(QString,QString);

public:
    explicit loginForm(QWidget *parent = 0);
    ~loginForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void connect_success();
    void disconect();
    void myrecv();

    void set_creat(QString anc,QString pass_word);
    void set_modify(QString anc,QString oldpass,QString newpass);

    void on_pbmini_login_clicked();

    void on_pbclose_login_clicked();

private:
    Ui::loginForm *ui;
    QTimer *timer;
public:
    modify_pass *Modify;
public:
    creat_anc *Creat;
    QTcpSocket *sock;
    error *War;
    void login_show();
    void net_set();
};

#endif // LOGINFORM_H
