#include "ui_loginwidget.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "window.h"
#include "game/map.h"
#include "game/objects/player.h"

#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QGlobalStatic>

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
    QLineEdit * username = findChild<QLineEdit*>("username");
    if (username->text().size() > 0) {
        findChild<QLineEdit*>("password")->setFocus();
    } else {
        username->setFocus();
    }
}

void LoginWidget::pause() {

}

void LoginWidget::on_pushButton_clicked() {
    QLineEdit * username = findChild<QLineEdit*>("username");
    QLineEdit * password = findChild<QLineEdit*>("password");

    const Json data = Json::object {
        {"Username", username->text().toStdString()},
        {"Password", password->text().toStdString()}
    };
    password->clear();

    if(connection::output(data) == false) {
        popupBox("Connection failed!");
    } else {
        checkResult();
    }
}

void LoginWidget::checkResult() {
    std::string input = connection::readPacket(5000);
    std::string error;
    Json data = Json::parse(input, error);

    std::cout << "LOGINWIDGET: " << data.dump() << std::endl;

    if(data["Type"].string_value() == "Login_Success") {
        if(data["Success"].bool_value() == true) {
            _self = new Self(std::move(map::parse_player(data["Player"])));
            Window *w = dynamic_cast<Window *> (this->parentWidget());
            w->setGameUi();
        }
        else
            popupBox("Login failed!");
    }
}

void LoginWidget::popupBox(const QString message) {
    QMessageBox::information(this, "", message);
}
