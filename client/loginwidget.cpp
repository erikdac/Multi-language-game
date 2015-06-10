#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "connection.hpp"
#include "json11/json11.hpp"

#include <iostream>
#include <QLineEdit>
#include <QMessageBox>

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

    QLineEdit * username = findChild<QLineEdit*>("username");
    QLineEdit * password = findChild<QLineEdit*>("password");

    Json data = Json::object {
        {"Username", username->text().toStdString()},
        {"Password", password->text().toStdString()}
    };

    output(data);

    login_mutex.lock();

    QMessageBox::information(
        this,
        tr("Application Name"),
        tr("An information message.") );
}
