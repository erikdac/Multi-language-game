#include "ui_loginwidget.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "window.h"
#include "config.h"

#include <QtDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>

using namespace json11;

LoginWidget::LoginWidget(QWidget * parent) :
    ui(new Ui::LoginWidget)
{
    this->setParent(parent);
    ui->setupUi(this);
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::resume() {
    // qinfo("Resumed");
    QLineEdit * username = findChild<QLineEdit*>("username");
    if (username->text().size() > 0) {
        findChild<QLineEdit*>("password")->setFocus();
    } else {
        username->setFocus();
    }
}

void LoginWidget::pause() {
    // qInfo("Paused");
}

void LoginWidget::tick(float deltaTime) {

}

void LoginWidget::on_pushButton_clicked() {
    QLineEdit * username = findChild<QLineEdit*>("username");
    QLineEdit * password = findChild<QLineEdit*>("password");

    const Json data = Json::object {
        {"Username", username->text().toStdString()},
        {"Password", password->text().toStdString()}
    };
    password->clear();

    const std::string response = connection::authenticate(IP, AUTHENTICATION_PORT, data);
    checkResult(response);
}

void LoginWidget::checkResult(const std::string & response) {
    if (response.empty()) {
        popupBox("Connection failed!");
    } else {
        std::string error;
        Json data = Json::parse(response, error);
        if (!error.empty()) {
            qWarning() << "Error in JSON recieved: " << error.c_str();
            popupBox("Connection refused!");
        } else if(data["Type"].string_value() == "Token") {
            std::string token = data["Value"].string_value();
            if (token == "") {
                popupBox("Login failed!");
            } else {
                qDebug() << "Authentication token = " << token.c_str();
                connection::run(token);
                dynamic_cast<Window *> (this->parentWidget())->setLoadingUi();
            }
        } else {
            qWarning() << "Recieved incorrect authentication package!";
        }
    }
}

void LoginWidget::popupBox(const QString message) {
    QMessageBox::information(this, "", message);
}
