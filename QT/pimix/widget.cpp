/*
 * @Author: CanEve 2785546297@qq.com
 * @Date: 2023-05-15 01:38:23
 * @LastEditors: CanEve 2785546297@qq.com
 * @LastEditTime: 2023-06-03 23:32:57
 * @FilePath: \undefinedf:\qtprojects\pimix\pimix\widget.cpp
 * @Description: 没有使用ui文件设置，直接cpp中添加的控件，图片合成数据打了9折（只测试了8位的bmp）
 */
#include "widget.h"
#include <QFile>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <string.h>
#include <stdio.h>
#include <QProgressBar>
#include <QSlider>

IMAGE image;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //2.
    lb_1 = new QLabel("+");
    lb_2 = new QLabel("=");
    lb_pix1 = new QLabel();
    lb_pix1->setFixedSize(340, 240);//源图片1显示label
    lb_pix2 = new QLabel();
    lb_pix2->setFixedSize(340, 240);//源图片2显示label
    lb_pix3 = new QLabel();
    lb_pix3->setFixedSize(340, 240);//合成图片显示label
    le = new QLineEdit();//调节比例的滑动条
    bt_1 = new QPushButton("图片1");
    bt_2 = new QPushButton("图片2");
    bt_3 = new QPushButton("合成");
    sl=new QSlider();
    sl->setMaximum(100);
    sl->setMinimum(0);
    sl->setValue(50);
    sl->setMinimumHeight(300);
    sl->setTracking(true);

    // sl2=new QSlider();
    // sl2->setMaximum(50);
    // sl2->setMinimum(-50);
    // sl2->setValue(0);

    lb_2->setText("= "+QString::number((sl->value())/100.0));



    //3个垂直排布
    QVBoxLayout *vbox1 = new QVBoxLayout;
    QVBoxLayout *vbox2 = new QVBoxLayout;
    QVBoxLayout *vbox3 = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;//将3个垂直排布装入水平排布中
    hbox->addLayout(vbox1);
    hbox->addWidget(lb_1);
    hbox->addLayout(vbox2);
    hbox->addWidget(lb_2);
    hbox->addWidget(sl);
//    hbox->addWidget(sl2);
    hbox->addLayout(vbox3);


    vbox1->addWidget(lb_pix1);//垂直排布分布放图片和选择按键
    vbox1->addWidget(bt_1);

    vbox2->addWidget(lb_pix2);
    vbox2->addWidget(bt_2);

    vbox3->addWidget(lb_pix3);

    vbox3->addWidget(bt_3);

    setLayout(hbox);
//    setLayout(vbox);
//    setLayout(hbox);

    //关联信号与槽
    connect(bt_1, SIGNAL(clicked(bool)), this, SLOT(desplay1()));
    connect(bt_2, SIGNAL(clicked(bool)), this, SLOT(desplay2()));
    connect(bt_3, SIGNAL(clicked(bool)), this, SLOT(desplay3()));
    connect(sl,SIGNAL(valueChanged(int)),this,SLOT(ValueChanged()));
    connect(sl,SIGNAL(sliderReleased()),this,SLOT(change_ok()));


}

Widget::~Widget()
{

}


void Widget::ValueChanged(){//改变比例时合成
    lb_2->setText("= "+QString::number((sl->value())/100.0)/*+" "+QString::number((sl2->value()))*/);
    desplay3();//
}

void Widget::desplay1()//选择图片1
{
    image.file1 = QFileDialog::getOpenFileName();//选择文件

    lb_pix1->setScaledContents(true);//自适应内容
    lb_pix1->setPixmap(image.file1);

}
void Widget::desplay2()//选择图片2
{
    image.file2 = QFileDialog::getOpenFileName();

    lb_pix2->setScaledContents(true);//自适应内容
    lb_pix2->setPixmap(image.file2);
}

void Widget::desplay3()//合成显示图片3
{
    QFile file1(image.file1);
    QFile file2(image.file2);
//    if(strlen(image.file3)<2)
    if(image.file3.isEmpty())
    image.file3=QFileDialog::getSaveFileName();
    QFile file3(image.file3);

    file1.open(QIODevice::ReadOnly);
    file2.open(QIODevice::ReadOnly);
    file3.open(QIODevice::ReadWrite|QIODevice::Text);
    file1.seek(0);
    QByteArray buf1=file1.readAll();
    file2.seek(0);
    QByteArray buf2=file2.readAll();
//    QByteArray buf3;
//    char buf4[60000]={0};


    int size=buf1.length()>buf2.length() ? buf2.length(): buf1.length();
    for(int i=54;i<size;i++){//图片叠加

            buf2[i]=(((sl->value())/100.0)*/*((sl2->value())/50.0))**/((unsigned char)buf1[i])+(1-((sl->value())/100.0))*/*((sl2->value())/50.0))**/((unsigned char)buf2[i]))*0.9+255*0.1;
            //这里对合成数据打了9折，不知道为什么原数据合成图片会花，打折后就正常了

    }
    file3.seek(0);
    file3.write(buf2);
    file3.close();
    file2.close();
    file1.close();


    lb_pix3->setScaledContents(true);//自适应内容
    lb_pix3->setPixmap(image.file3);


}
void Widget::change_ok(){
    
}
