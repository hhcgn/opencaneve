/*
 * @Author: CanEve 2785546297@qq.com
 * @Date: 2023-06-02 00:38:14
 * @LastEditors: CanEve 2785546297@qq.com
 * @LastEditTime: 2023-06-02 00:51:55
 * @FilePath: \undefinedd:\Git\GIT_UPDATE\opencaneve\opencaneve\QT\tcp_cam\tcp_cam\widget.cpp
 * @Description: qt主界面
 */
#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton_2->setDisabled(true);
    flag=0;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if((ui->lineEdit->text().isEmpty())||(ui->lineEdit_2->text().isEmpty())){
        return;
    }
    sock = new QTcpSocket;
    //sock->waitForConnected(3000);
    sock->connectToHost(ui->lineEdit->text(),ui->lineEdit_2->text().toInt());
    connect(sock, SIGNAL(readyRead()), this, SLOT(recv_pix_show()));
    connect(sock, SIGNAL(connected()), this, SLOT(connect_success()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(disconect()));
    connect(sock,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this,SLOT(sock_error(QAbstractSocket::SocketError)));
}
void Widget::recv_pix_show()
{


    //1.接受图像数据


    QByteArray pic_size;

    size_i=0;


    if(2==flag) goto show;
    else if(0==flag){
    size_pic_i=0;
    pic_size=sock->read(6);
    size_pic_i=atoi(pic_size);
    }
    if(sock->bytesAvailable()>=size_pic_i){
        buf=sock->read(size_pic_i);
        flag=2;
    }else {
        flag=1;
        return;
    }

    show:
    //if(tim_count>2){
    flag=0;
    QPixmap image_show;
    image_show.loadFromData(buf);
   qDebug()<<"show";
    ui->label_3->setPixmap(/*QPixmap("1.jpg")*/image_show);
    buf.clear();

    update();
   // }

}
void Widget::connect_success(){
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(false);
    qDebug()<<"connected";
}
void Widget::disconect(){
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(true);
     qDebug()<<"disconnected";
}


void Widget::on_pushButton_2_clicked()
{
    if(sock->isOpen()){
        sock->close();
        qDebug()<<"opened";
    }else qDebug()<<"closed";
}
void Widget::sock_error(QAbstractSocket::SocketError){
    QMessageBox box;
            box.setStyleSheet("QLabel{"
                              "min-width: 100px;"
                              "min-height: 100px; "
                              "font-size:20px;"
                              "}"
                              "QPushButton {"
                              "background-color:#89AFDE;"
                              " border-style: outset;"
                              " border-width: 15px;"
                              " border-radius: 10px;"
                              " border-color: beige;"
                              " font: bold 10px;"
                              " min-width: 15em;"
                              " min-height: 5em;"
                              "}"
                              "");//此处设置弹窗的字体和按钮属性
            box.setText("出错，可能ip或者\n端口有问题");//设置显示文本
            box.setWindowTitle("提示");//设置弹窗标题
            box.exec();



}

