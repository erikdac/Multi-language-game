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
    void popupBox(const QString);

public slots:
    void input(std::string result);

private slots:
    void on_pushButton_clicked();

};

#endif
