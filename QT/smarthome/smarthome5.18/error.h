#ifndef ERROR_H
#define ERROR_H

#include <QWidget>

namespace Ui {
class error;
}

class error : public QWidget
{
    Q_OBJECT

public:
    explicit error(QWidget *parent = nullptr);
    ~error();

private slots:
    void on_pushButton_clicked();

private:
    Ui::error *ui;
};

#endif // ERROR_H
