#include "eyeform.h"
#include "ui_eyeform.h"
#include "jiemian.h"
#include <QFile>
#include <string.h>
#include <QTimer>

EyeForm::EyeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EyeForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        //隐藏窗口的标题栏



    sock = new QTcpSocket;
    sock->connectToHost(/*"192.168.80.128"*/"192.168.137.2", 8887);
    connect(sock, SIGNAL(readyRead()), this, SLOT(recv_pix_show()));
    // 产生一个定时器
    QTimer *timer = new QTimer(this);

    //连接这个定时器的信号和槽，利用定时器的timeout(),从而触发clear()槽函数
    connect(timer, &QTimer::timeout, this, &EyeForm::show_pic);

    //开始定时器，并设定定时周期,每隔5秒就会重启定时器，会重复触发定时，除非你利用stop()将定时器关掉
    timer->start(100);
    tim_count=0;
    flag=0;

    // 仅仅启动定时器一次
    //timer->setSingleShot(true);

}

EyeForm::~EyeForm()
{
    delete ui;

}

void EyeForm::recv_pix_show()
{
    //disconnect(sock,0,0,0);

    //1.接受图像数据


    QByteArray pic_size;

    size_i=0;


    if(2==flag) goto show;
    else if(0==flag){
    size_pic_i=0;
    pic_size=sock->read(6);
    size_pic_i=atoi(pic_size);
    }
    if(sock->bytesAvailable()>size_pic_i){
        buf=sock->read(size_pic_i);
        flag=2;
    }else {
        flag=1;
        return;
    }

    show:
    if(tim_count>2){
    flag=0;
    QPixmap image_show;
    image_show.loadFromData(buf);
   qDebug()<<"show";
    ui->label_2->setPixmap(/*QPixmap("1.jpg")*/image_show);
    buf.clear();

    update();
    }
    //qDebug()<<"rec length: ";
    /*qDebug()<<buf.length();
    if(3==flag){
    buf.truncate(0);
    buf.clear();
    buf.chop(buf.length());

    qDebug()<<"clear";
    return;
}else {
        buf+= sock->readAll();
    }
    if(buf.length()<12) return;

    for(long i=0;i<buf.length();i++){
        if((buf[i]=='c'&&buf[i+1]=='a'&&buf[i+2]=='n'&&buf[i+3]=='e'&&buf[i+4]=='v'&&buf[i+5]=='e')&&0==flag){
            qDebug()<<"find";
            flag=1;
            i=i+5;
            qDebug()<<i;
        }
        else
        if(1==flag){
            qDebug()<<"caneve";
            pic_size+=buf[i];
            size_i++;
            if(size_i==6){
                qDebug()<<"size"+pic_size;
                flag=2;
                size_i=0;
                size_pic_i=atoi(pic_size);
                if(buf.length()<i+size_pic_i) {
                    next_flag=1;
                    qDebug()<<"return";
                    return;
                }else{
                    next_flag=0;
//                    qDebug()<<"i";
//                    qDebug()<<i;
//                    qDebug()<<buf;

                }

                //;
                }
        }else if(2==flag){
            img_buf+=buf[i];
            size_i++;
            if(size_i==size_pic_i){
                qDebug()<<"img_buf";
//                qDebug()<<img_buf;
                flag=3;
                //disconnect(sock,0,0,0);
                break;
            }
        }
    }
*/
//    long size=atoi(buf);
//    qDebug()<<buf;
//    //if(size<=0)
//    while(sock->bytesAvailable()<size);

//    buf=sock->read(size);
//    if(){

//    }else{
//       //return;
//    }
    /*QFile file("1.jpg");
    file.open(QIODevice::WriteOnly);
    file.write(buf);
    file.close();*/

    //2.显示

    //connect(sock, SIGNAL(readyRead()), this, SLOT(recv_pix_show()));
}
void EyeForm::show_pic(){
tim_count++;
// if(flag==3){
// QPixmap image_show;
// image_show.loadFromData(img_buf);
//qDebug()<<"show";
// ui->label_2->setPixmap(/*QPixmap("1.jpg")*/image_show);
// img_buf.clear();
// update();
// flag=0;
// }
}

void EyeForm::on_pushButton_clicked()
{
    //发出请求，关闭监控
    hide();
    emit cam_back();
    sock->close();
    close();
    //关闭监控页面，返回主页面
    //jiemian *Jiemian=new jiemian;  //给窗口2创建一个对象
    //this ->close();                //this是当前窗体（窗口1）的指针，关闭
    //Jiemian ->show();
}
void EyeForm::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void EyeForm::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void EyeForm::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}
