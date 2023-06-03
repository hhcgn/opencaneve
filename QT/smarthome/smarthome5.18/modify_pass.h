#ifndef MODIFY_PASS_H
#define MODIFY_PASS_H

#include <QDialog>

namespace Ui {
class modify_pass;
}

class modify_pass : public QDialog
{
    Q_OBJECT
signals:
    void modify_passw(QString a,QString b,QString c);
    void modify_close();
public:
    explicit modify_pass(QWidget *parent = 0);
    ~modify_pass();


private slots:
    void on_pushButton_clicked();

private:
    Ui::modify_pass *ui;

public:
    void modify_ok();
    void modify_fail();
};

#endif // MODIFY_PASS_H
