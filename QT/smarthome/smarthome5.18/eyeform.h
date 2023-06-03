#ifndef EYEFORM_H
#define EYEFORM_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class EyeForm;
}

class EyeForm : public QWidget
{
    Q_OBJECT
signals: //我支持的信号
    void cam_back();
public:
    explicit EyeForm(QWidget *parent = 0);
    ~EyeForm();

private slots:
    void on_pushButton_clicked();

    void recv_pix_show();
    void show_pic();

private:
    Ui::EyeForm *ui;
    QTcpSocket *sock;
    QByteArray buf,img_buf;
    int flag,size_i,size_pic_i,next_flag=0,tim_count;
    QPoint last;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // EYEFORM_H
