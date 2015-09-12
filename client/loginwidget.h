#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <iostream>
#include <QWidget>
#include <mutex>

extern std::mutex login_mutex;

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QWidget {

    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

private:
    Ui::LoginWidget *ui;

public slots:
    void input(std::string);

private slots:
    void on_pushButton_clicked();


};

#endif // LOGINWIDGET_H
