#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "connection.hpp"
#include "json11/json11.hpp"

#include <iostream>

using namespace json11;

std::mutex login_mutex;

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWidget) {
    ui->setupUi(this);
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::on_pushButton_clicked()
{
    login_mutex.lock();
    Json data = Json::object {
        {"Username", "erik"},
        {"Password", "no"}
    };

    output(data);

    login_mutex.lock();
}
