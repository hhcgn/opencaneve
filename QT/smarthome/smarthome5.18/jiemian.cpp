#include "jiemian.h"
#include "ui_jiemian.h"
#include "eyeform.h"
#include <QMovie>
#include <QLabel>

#include <QDesktopServices>
#include <QUrl>

jiemian::jiemian(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jiemian)
{
    ui->setupUi(this);

    loginW = new loginForm;
    loginW->show();

    connect(loginW, SIGNAL(login_successed()), this, SLOT(show()));

    on_connect();

    ui->pushButton_LED->show();
       ui->pushButton_LED_2->hide();

       ui->pushButton_LED31->show();
       ui->pushButton_LED32->hide();

       ui->pushButton_LED41->show();
       ui->pushButton_LED42->hide();

       ui->pushButton_waring->show();
       ui->pushButton_waring_2->hide();
       ui->label_shan->hide();
       ui->pushButton_lighting->show();
       ui->pushButton_lighting2->hide();
       ui->pushButton_music->show();
       ui->pushButton_musicing->hide();
       ui->pbmini->show();
       ui->pbclose->show();
       //setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
       //隐藏窗口的标题栏
}

jiemian::~jiemian()
{
    delete ui;
}


void jiemian::on_pushButton_LED_clicked()
{
    //灯泡第一次按键，请求亮灯
    on_pub("led1");

    //灯泡亮


}

void jiemian::on_pushButton_LED_2_clicked()
{
    //灯泡第二次按键，请求灭灯
    on_pub("led1");

    //灯泡灭

}
void jiemian::on_pushButton_LED31_clicked()
{
    //打开灯泡2
    //灯泡第一次按键，请求亮灯
    on_pub("led2");

    //灯泡亮

}

void jiemian::on_pushButton_LED32_clicked()
{
    //关闭灯泡2
    //灯泡第二次按键，请求灭灯
    on_pub("led2");
    //灯泡灭

}

void jiemian::on_pushButton_LED41_clicked()
{
    //打开灯泡3
    //灯泡第一次按键，请求亮灯

    on_pub("led2");
    //灯泡亮

}


void jiemian::on_pushButton_LED42_clicked()
{
    //关闭灯泡3
    //灯泡第二次按键，请求灭灯

    //灯泡灭
    on_pub("led2");
}


void jiemian::on_pushButton_eye_clicked()
{
    //按键请求打开监控页面


    //打开监控
    EyeForm *eyeform=new EyeForm;  //给监控2创建一个对象
    hide();
    //this ->close();                //this是当前主界面的指针，关闭
    eyeform ->show();              //监控显示出来
    connect(eyeform, SIGNAL(cam_back()), this, SLOT(show()));

}

void jiemian::on_pushButton_waring_clicked()
{
    //打开报警，实时检测是否收到警示信息



    on_pub("beep");

}


void jiemian::on_pushButton_waring_2_clicked()
{
    //关闭警报
   on_pub("beep");
}



void jiemian::on_pushButton_lighting_clicked()
{
    //闪光灯亮
   on_pub("flash");
}

void jiemian::on_pushButton_lighting2_clicked()

{
    //闪光灯灭
    on_pub("flash");
}


void jiemian::on_pushButton_3D_clicked()
{
    //打开3D界面
    QDesktopServices::openUrl(QUrl("https://yyzlab.com.cn/course/info/1580391402731810817"));
}

void jiemian::on_pushButton_music_clicked()
{
    ui->pushButton_musicing->show();
    ui->pushButton_music->hide();
    ui->label_shan->show();
    QMovie * movie = new QMovie(this);//播放gif
    movie->setFileName(":/photo/listening.gif");
    movie->start();//播放
    ui->label_musicgif->setMovie(movie);
}

void jiemian::on_pushButton_musicing_clicked()
{
    ui->pushButton_music->show();
    ui->pushButton_musicing->hide();
}



void jiemian::on_pbmini_clicked()
{
    this->showMinimized();
}

/** 重绘窗体,解决最小化后在恢复，界面不更新的问题 */
void jiemian::showEvent(QShowEvent *e)
{
    this->setAttribute(Qt::WA_Mapped);
    QWidget::showEvent(e);
}
void jiemian::on_pbclose_clicked()
{
    this->close();
    emit all_close();
}
void jiemian::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}
void jiemian::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void jiemian::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


void jiemian::on_pushButton_3Dhome_clicked()
{

    QDesktopServices::openUrl(QUrl("https://yyzlab.com.cn/course/info/1580391402731810817"));

}
void jiemian::set_temp(int temp,int  hum){

    ui->label_shidu->setText(QString::number(100*hum/250.0)+"%");
    //湿度条
    ui->lab_temp2_2->setGeometry(210,380,hum*(380/260.0),90);
    if (hum*(380/260.0) <= 330 & hum*(380/260.0) >= 250)
         ui->lab_temp2_2->setStyleSheet( "background-color: rgb(255, 170, 0);""border-radius:20px;");
    else if (hum*(380/260.0) > 300)
         ui->lab_temp2_2->setStyleSheet( "background-color: rgb(255, 0, 0);""border-radius:20px;");
    else
         ui->lab_temp2_2->setStyleSheet( "background-color: rgb(208, 254, 204);""border-radius:20px;");




    ui->label_wendu->setText(QString::number(temp));
    if(temp>40) temp=40;
    //温度条
    ui->lab_temp3_3->setGeometry(210,490,temp*(380/55.0),90);
    if ((temp*(380/55.0)) <= 304 & (temp*(380/55.0)) >= 266)
         ui->lab_temp3_3->setStyleSheet( "background-color: rgb(255, 170, 0);""border-radius:20px;");
    else if (temp*(380/55.0) > 304)
         ui->lab_temp3_3->setStyleSheet( "background-color: rgb(255, 0, 0);""border-radius:20px;");
    else
         ui->lab_temp3_3->setStyleSheet( "background-color: rgb(208, 254, 204);""border-radius:20px;");
}




void jiemian::on_connect()
{
    client = new QMqttClient;
    //绑定连接成功信号
    connect(client, &QMqttClient::connected, this, &jiemian::connectSuccess);
    connect(client, &QMqttClient::disconnected, this, &jiemian::connectError);
    //绑定连接的ip
    client->setHostname(/*ui->ip_lineEdit->text()*/"81.70.2.209");
    //绑定端口
    client->setPort(/*ui->port_lineEdit->text().toUInt()*/1883);
    //连接服务器
    client->connectToHost();

    client2 = new QMqttClient;
    //绑定连接成功信号
    connect(client2, &QMqttClient::connected, this, &jiemian::connectSuccess2);
    connect(client2, &QMqttClient::disconnected, this, &jiemian::connectError2);
    //绑定连接的ip
    client2->setHostname(/*ui->ip_lineEdit->text()*/"81.70.2.209");
    //绑定端口
    client2->setPort(/*ui->port_lineEdit->text().toUInt()*/1883);
    //连接服务器
    client2->connectToHost();

    qDebug()<<"connect";
}

void jiemian::connectSuccess()
{
    //QMessageBox::about(this, "提示", "连接成功");
    qDebug()<<"connected";
    on_sub();
}
void jiemian::connectSuccess2()
{
    //QMessageBox::about(this, "提示", "连接成功");
    qDebug()<<"2connected";
    on_sub2();
}

void jiemian::connectError2()
{
    qDebug()<<"2error";
    //QMessageBox::about(this, "提示", "连接失败");
}
void jiemian::connectError()
{
    qDebug()<<"error";
    //QMessageBox::about(this, "提示", "连接失败");
}

void jiemian::on_sub2()
{
    //订阅主题
    client2->subscribe(QMqttTopicFilter("caneve0"));
    //client->subscribe(/*ui->sub_lineEdit->text()*/"caneve1");
    //绑定信号与槽，当收到消息后调用槽函数
    connect(client2, &QMqttClient::messageReceived, this, &jiemian::Receive2);
    //on_pub();
    qDebug()<<"2sub";
}

void jiemian::on_sub()
{
    //订阅主题
    client->subscribe(QMqttTopicFilter("can0"));
    //client->subscribe(/*ui->sub_lineEdit->text()*/"caneve1");
    //绑定信号与槽，当收到消息后调用槽函数
    connect(client, &QMqttClient::messageReceived, this, &jiemian::Receive);
    //on_pub();
    qDebug()<<"sub";
}

void jiemian::Receive2(QByteArray buf)
{
    //ui->textEdit->append(QString(buf));
    qDebug()<<"2recv ";
    qDebug()<<buf;

}
void jiemian::Receive(QByteArray buf)
{
    //ui->textEdit->append(QString(buf));
    qDebug()<<"recv ";
    qDebug()<<buf;
    int temp_i=0;
    int humi=0;
    if(strstr(buf,"led1")){
        if(strstr(buf,"led11")){
            on_pub2("{\"lamp\":true,\"id\":0}");
            ui->pushButton_LED->hide();
            ui->pushButton_LED_2->show();
        }else{
            on_pub2("{\"lamp\":false,\"id\":0}");
            ui->pushButton_LED->show();
            ui->pushButton_LED_2->hide();
        }

    }
    if(strstr(buf,"led2")){
        if(strstr(buf,"led21")){
            on_pub2("{\"lamp\":true,\"id\":1}");
            ui->pushButton_LED41->hide();
            ui->pushButton_LED42->show();
//            ui->pushButton_LED31->hide();
//            ui->pushButton_LED32->show();
        }else{
            on_pub2("{\"lamp\":false,\"id\":1}");
            ui->pushButton_LED41->show();
            ui->pushButton_LED42->hide();
//            ui->pushButton_LED31->show();
//            ui->pushButton_LED32->hide();
        }
    }
     if(strstr(buf,"flash")){
        if(strstr(buf,"flash1")) {
         ui->pushButton_lighting2->show();
         ui->pushButton_lighting->hide();}
        else{
            ui->pushButton_lighting->show();
            ui->pushButton_lighting2->hide();
        }

    }
     if(strstr(buf,"hum")){
         char hum[6]={0};
         char *s_start=strstr(buf,"hum");
         for(int i1=3;i1<6;i1++){
             if(*(s_start+i1)!='|')
             hum[i1-3]= *(s_start+i1);
             else break;
         }
         humi=atoi(hum);
         qDebug()<<humi;

    }
     if(strstr(buf,"beep")){
        if(strstr(buf,"beep1")){
            ui->pushButton_waring_2->show();
            ui->pushButton_waring->hide();


            //报警已打开，同时如果接到报警信息则播放警报gif
            ui->label_shan->show();
            QMovie * movie = new QMovie(this);//播放gif
            movie->setFileName(":/photo/warning2.gif");
            movie->start();//播放
            ui->label_shan->setMovie(movie);
        }else{
            ui->pushButton_waring->show();
            ui->pushButton_waring_2->hide();
        }

    }
     if(strstr(buf,"temp")){
         char temp_c[6]={0};
         char *s_start=strstr(buf,"temp");
         for(int i1=4;i1<6;i1++){
         if(*(s_start+i1)!='|')
         temp_c[i1-4]= *(s_start+i1);
         else break;
         }
         temp_i=atoi(temp_c);
         qDebug()<<temp_i;

    }
    set_temp(temp_i,humi);
}

void jiemian::on_pub(QByteArray msg)
{
    //发布消息，主题为pub_lineEdit
    client->publish(QMqttTopicName("can1"),msg);
    qDebug()<<"pub"+msg;
    //client->publish(/*ui->pub_lineEdit->text(), ui->text_lineEdit->text().toLatin1()*/QMqttTopicFilter("caneve0"),"msg");
}

void jiemian::on_pub2(QByteArray msg)
{
    //发布消息，主题为pub_lineEdit
    client2->publish(QMqttTopicName("caneve1"),msg);
    qDebug()<<"2pub"+msg;
    //client->publish(/*ui->pub_lineEdit->text(), ui->text_lineEdit->text().toLatin1()*/QMqttTopicFilter("caneve0"),"msg");
}

/*void jiemian::on_clear_Button_clicked()
{
    //ui->textEdit->clear();

}*/
