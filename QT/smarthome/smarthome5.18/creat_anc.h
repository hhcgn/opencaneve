#ifndef CREAT_ANC_H
#define CREAT_ANC_H

#include <QDialog>


namespace Ui {
class creat_anc;
}

class creat_anc : public QDialog
{
    Q_OBJECT
signals:

    void creat_anco(QString a,QString b);
public:
    explicit creat_anc(QWidget *parent = 0);
    ~creat_anc();



private slots:
    void on_pushButton_clicked();

private:
    Ui::creat_anc *ui;
public :
    void creat_ok();
    void creat_fail();

};

#endif // CREAT_ANC_H
