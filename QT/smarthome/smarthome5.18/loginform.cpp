#include "loginform.h"
#include "ui_loginform.h"
#include <QTimer>
#include <QApplication>
#include <QMessageBox>


loginForm::loginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginForm)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);


    Modify = new modify_pass;
//    Modify->show();

    Creat =new creat_anc;

    War =new error;
    //War->show();

    sock = new QTcpSocket; //构造套接字
    sock->connectToHost(/*"192.168.80.128"*/"192.168.137.2", 8888);//连接远方服务器


    timer = new QTimer(this);

    //连接这个定时器的信号和槽，利用定时器的timeout(),从而触发clear()槽函数
    connect(timer, &QTimer::timeout, this, &loginForm::net_set);

    //开始定时器，并设定定时周期,每隔5秒就会重启定时器，会重复触发定时，除非你利用stop()将定时器关掉
    timer->start(1000);

   // sock->close();


    connect(sock, SIGNAL(connected()), this, SLOT(connect_success()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(disconect()));
    connect(sock, SIGNAL(readyRead()), this, SLOT(myrecv()));

}

loginForm::~loginForm()
{
    delete ui;
}

void loginForm::on_pushButton_clicked()
{
    //emit login_signal(ui->lineEdit->text(), ui->lineEdit_2->text());
    if(!ui->lineEdit->text().isEmpty()&&!ui->lineEdit->text().isEmpty()){
    sock->write(("select "+ui->lineEdit->text()+" "+ui->lineEdit_2->text()).toStdString().c_str());
    }else{

    }
//    if(ui->lineEdit->text() == ui->lineEdit_2->text())
//    {

//        hide();
//        //主界面要呈现！！！我发出一个登陆成功信号
//        emit login_successed();
//    }
}

void loginForm::on_pushButton_3_clicked()  //修改密码
{
    hide();
    Modify->show();
    connect(Modify,SIGNAL(modify_close()),this,SLOT(show()));
    connect(Modify, SIGNAL(modify_passw(QString,QString,QString)), this, SLOT(set_modify(QString,QString,QString)));

}

void loginForm::on_pushButton_2_clicked()   //创建账号
{

    hide();
    Creat->show();
    connect(Creat, SIGNAL(creat_anco(QString,QString)), this, SLOT(set_creat(QString,QString)));
//
}
void loginForm::connect_success()
{
    //ui->pushButton->setStyleSheet("QPushButton{background-color: green;}");
    timer->stop();
}

void loginForm::disconect()
{
    //ui->pushButton->setStyleSheet("QPushButton{background-color: red;}");
    if(!this->isHidden())timer->start(1000);
}

void loginForm::myrecv()
{
    QByteArray buf = sock->readAll();
    if(0==strncmp(buf,"insertok",8)){
        qDebug()<<"insertok";
        show();
        QMessageBox::about(this, "提示", "注册成功");
        Creat->creat_ok();
    }else if(0==strncmp(buf,"insertfail",10)){
        qDebug()<<"insertfail";
        War->show();
        //Creat->creat_fail();
    }else if(0==strncmp(buf,"selectok",8)){
        qDebug()<<"selectok";
        hide();
        sock->close();
        emit login_successed();
    }else if(0==strncmp(buf,"selectfail",10)){
        qDebug()<<"selectfail";
        War->show();

    }else if(0==strncmp(buf,"updateok",8)){
        qDebug()<<"updateok";
        Modify->modify_ok();

        show();
         QMessageBox::about(this, "提示", "修改成功");
    }else if(0==strncmp(buf,"updatefail",10)){
        qDebug()<<"updatefail";
        Modify->modify_fail();
        War->show();
    }else if(0==strncmp(buf,"deleteok",8)){
        qDebug()<<"deleteok";
    }else if(0==strncmp(buf,"deletefail",10)){
        qDebug()<<"deletefail";
    }
//    ui->textEdit->append(buf);
//    ui->textEdit->setText(buf);

}
void loginForm::set_creat(QString anc,QString pass_word){
   qDebug()<<"creat<login\n";
   sock->write(("insert "+anc+" "+pass_word).toStdString().c_str());
    //emit back_c_data(anc,pass_word);
}
void loginForm::set_modify(QString anc,QString oldpass,QString newpass){
    qDebug()<<"modify<login\n";
    sock->write(("update "+anc+" "+oldpass+" "+newpass).toStdString().c_str());
    //emit back_m_data(anc,oldpass,newpass);
}
void loginForm::net_set(){
    qDebug()<<"net_con";
    if(!sock->isOpen()){

         sock->connectToHost(/*"192.168.80.128"*/"192.168.137.2", 8888);//连接远方服务器
         connect(sock, SIGNAL(connected()), this, SLOT(connect_success()));
         connect(sock, SIGNAL(disconnected()), this, SLOT(disconect()));
         connect(sock, SIGNAL(readyRead()), this, SLOT(myrecv()));
         qDebug()<<"net_con";
    }

}
void loginForm::on_pbmini_login_clicked()
{
    this->showMinimized();
}

void loginForm::on_pbclose_login_clicked()
{
//   QApplication* app;
//   app->exit(0);
    this->close();
    delete ui;
    //emit all_close();
}
