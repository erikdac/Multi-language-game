#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "mainwindow.h"
#include "network/network_reader.h"

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

void LoginWidget::on_pushButton_clicked() {

    QLineEdit * username = findChild<QLineEdit*>("username");
    QLineEdit * password = findChild<QLineEdit*>("password");

    Json data = Json::object {
        {"Username", username->text().toStdString()},
        {"Password", password->text().toStdString()}
    };

    output(data);

//    login_mutex.lock();

 //   if(!online()) {
        QMessageBox::information(
            this,
            tr("Application Name"),
            tr("An information message.")
        );
//    }
//    else {
//        ((MainWindow*)parentWidget())->MainWindow::setUpGameUi();
//    }
}

void LoginWidget::input(std::string result) {
    if(result[0] == 1)
        std::cout << "JAAAAAAA" << std::endl;
    else
        std::cout << result << std::endl;
}
