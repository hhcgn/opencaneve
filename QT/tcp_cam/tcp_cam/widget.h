#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void recv_pix_show();

    void on_pushButton_2_clicked();

    void connect_success();
    void disconect();
    void sock_error(QAbstractSocket::SocketError);

private:
    Ui::Widget *ui;
public:
    QTcpSocket *sock;
    QByteArray buf,img_buf;
    int flag,size_i,size_pic_i,next_flag=0,tim_count;
    QPoint last;
};
#endif // WIDGET_H
