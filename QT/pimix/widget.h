#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>

class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void desplay1();
    void desplay2();
    void desplay3();
    void ValueChanged();
    void change_ok();
public:
    Widget(QWidget *parent = 0);
    ~Widget();



    //1.满足畅想
    QLabel *lb_1;
    QLabel *lb_2;
    QLabel *lb_pix1;
    QLabel *lb_pix2;
    QLabel *lb_pix3;
    QLineEdit *le;
    QPushButton *bt_1;
    QPushButton *bt_2;
    QPushButton *bt_3;
    QSlider *sl;
    QSlider *sl2;



};
typedef struct
{
    QString file1,file2,file3;
}IMAGE;


#endif // WIDGET_H
