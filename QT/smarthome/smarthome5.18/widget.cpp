#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    loginW = new loginForm;
    loginW->show();

    connect(loginW, SIGNAL(login_successed()), this, SLOT(show()));
}

Widget::~Widget()
{
    delete ui;
}
