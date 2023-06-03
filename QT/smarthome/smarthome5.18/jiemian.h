#ifndef JIEMIAN_H
#define JIEMIAN_H

#include <QMainWindow>
#include <QPushButton>
#include "loginform.h"
#include <QMouseEvent>

#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include <QMessageBox>
namespace Ui {
class jiemian;
}

class jiemian : public QMainWindow
{
    Q_OBJECT

public:
    explicit jiemian(QWidget *parent = 0);
    ~jiemian();
signals:
    void all_close();
private slots:

    void on_pushButton_LED_clicked();
        void on_pushButton_eye_clicked();
        void on_pushButton_waring_clicked();
        void on_pushButton_waring_2_clicked();
        void on_pushButton_lighting_clicked();
        void on_pushButton_lighting2_clicked();
        void on_pushButton_3D_clicked();
        void on_pushButton_music_clicked();
        void on_pushButton_musicing_clicked();
        void on_pbmini_clicked();
        void on_pbclose_clicked();
        void on_pushButton_LED_2_clicked();

        void on_pushButton_3Dhome_clicked();

        void on_pushButton_LED41_clicked();

        void on_pushButton_LED42_clicked();

        void on_pushButton_LED31_clicked();

        void on_pushButton_LED32_clicked();


        void connectSuccess();

        void connectError();

        void Receive(QByteArray buf);

        void connectSuccess2();

        void connectError2();

        void Receive2(QByteArray buf);


private:
    Ui::jiemian *ui;
    loginForm *loginW;
    QMqttClient *client;
    QMqttClient *client2;

    QPoint last;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void showEvent(QShowEvent *e);

public:
    void on_pub(QByteArray msg);
    void on_sub();
    void on_connect();
    void on_pub2(QByteArray msg);
    void on_sub2();
    void on_connect2();
    void set_temp(int,int);

};

#endif // JIEMIAN_H
