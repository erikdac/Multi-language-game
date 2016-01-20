#include "ui_loginwidget.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "mainwindow.h"
#include "game/map.h"
#include "game/objects/player.h"

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
    _online = true;
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

void LoginWidget::input(std::string input) {
    std::string error;
    Json data = Json::parse(input, error);

    if(data["Type"].string_value() == "LoginSuccess") {
        if(data["Success"].string_value() == "yes") {
            _online = true;
        }
        else
            popupBox("Login failed!");
    }
    else if(data["Type"].string_value() == "Player" && _online) {
        int x = std::stoi(data["x"].string_value());
        int y = std::stoi(data["y"].string_value());
        _player = new Player(x, y);
        MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
        w->setUpGameUi();
    }
}

void LoginWidget::popupBox(const QString message) {
    QMessageBox::information(this, "", message);
}
