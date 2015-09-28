#include "ui_loginwidget.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "mainwindow.h"

#include <iostream>
#include <QLineEdit>
#include <QMessageBox>

using namespace json11;

std::mutex login_mutex;

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    findChild<QLineEdit*>("username")->setFocus();
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::on_pushButton_clicked() {

    const QLineEdit * username = findChild<QLineEdit*>("username");
    const QLineEdit * password = findChild<QLineEdit*>("password");

    const Json data = Json::object {
        {"Username", username->text().toStdString()},
        {"Password", password->text().toStdString()}
    };

    if(connection::output(data) == false) {
        popupBox("Connection failed!");
    }
}

void LoginWidget::input(std::string result) {
    std::string error;
    Json data = Json::parse(result, error);
    if(data["Success"].string_value().compare("yes") == 0) {
        MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
        w->setUpGameUi();
    }
    else
        popupBox("Login failed!");
}

void LoginWidget::popupBox(const QString message) {
    QMessageBox::information(this, "", message);
}
