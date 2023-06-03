#include "modify_pass.h"
#include "ui_modify_pass.h"
#include "loginform.h"


modify_pass::modify_pass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modify_pass)
{
    ui->setupUi(this);

}

modify_pass::~modify_pass()
{
    emit modify_close();
    delete ui;
}

void modify_pass::on_pushButton_clicked()
{
    QString count;
    QString oldpass;
    QString pass;
 if((!ui->lineEdit->text().isEmpty())&& (!ui->lineEdit_2->text().isEmpty()) )        //输入的账号和密码要与云端相同
 {
        //hide();
        count=ui->lineEdit->text();
        oldpass=ui->lineEdit_2->text();
        pass=ui->lineEdit_3->text();                       //将新密码覆盖掉旧密码
        emit modify_passw(count,oldpass,pass);
 }else{

 }
}
void modify_pass::modify_ok(){
    hide();
}
void modify_pass::modify_fail(){
    qDebug()<<"modifyfail";
}
