#include "ui_loginwidget.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "mainwindow.h"
#include "game/map.h"
#include "game/objects/player.h"

#include <iostream>
#include <QLabel>
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

    std::cout << "LOGINWIDGET: " << data.dump() << std::endl;

    if(data["Type"].string_value() == "Login_Success") {
        if(data["Success"].bool_value() == true) {
            _self = new Player(std::move(map::parse_player(data["Player"])));
            MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
            w->setUpGameUi();
            _online = true;
            connection::output(Json::object{{"Type", "Ready"}});
        }
        else
            popupBox("Login failed!");
    }
}

void LoginWidget::popupBox(const QString message) {
    QMessageBox::information(this, "", message);
}
