#include "creat_anc.h"
#include "ui_creat_anc.h"


creat_anc::creat_anc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::creat_anc)
{
    ui->setupUi(this);

//    connect(creat, SIGNAL(creat_anco()), this, SLOT(show()));
}

creat_anc::~creat_anc()
{
    delete ui;
}

void creat_anc::on_pushButton_clicked()
{
    QString  anc,pass_word;
    if(!ui->lineEdit->text().isEmpty()&&!ui->lineEdit_2->text().isEmpty()){
    anc=ui->lineEdit->text();
    pass_word=ui->lineEdit_2->text();               //将账号密码发送给云端
//    qDebug()<<"ans "+anc;
    emit creat_anco(anc,pass_word);
    }else{

    }
}
void creat_anc::creat_ok(){
    hide();
}
void creat_anc::creat_fail(){
    qDebug()<<"creatfail";
}

