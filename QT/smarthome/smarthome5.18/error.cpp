#include "error.h"
#include "ui_error.h"

error::error(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::error)
{
    ui->setupUi(this);
}

error::~error()
{
    delete ui;
}

void error::on_pushButton_clicked()
{
    hide();
}

