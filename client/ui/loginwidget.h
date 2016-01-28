#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QWidget {

    Q_OBJECT
    bool _online;

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

private:
    Ui::LoginWidget *ui;
    void popupBox(const QString);

public slots:
    void input(std::string);

private slots:
    void on_pushButton_clicked();

};

#endif
