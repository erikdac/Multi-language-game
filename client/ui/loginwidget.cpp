#include "ui_loginwidget.h"
#include "loginwidget.h"
#include "network/connection.h"
#include "external/json11/json11.hpp"
#include "window.h"
#include "game/gamestruct.h"

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

    if (isConnected()) {
        if(connection::output(data) == false) {
            popupBox("Connection failed!");
        } else {
            checkResult();
        }
    }
}

/**
 * Needed to detect if the connection with the server is alive or not.
 *
 * @return True, if a connection exists.
 */
bool LoginWidget::isConnected() {
    if (connection::readPacket(1) != "") {
        if (connection::connectToServer() == false) {
            connection::disconnect();
            return false;
        }
    }
    return true;
}

void LoginWidget::checkResult() {
    std::string input = connection::readPacket(5000);
    std::string error;
    Json data = Json::parse(input, error);
    if (!error.empty()) {
        popupBox("Connection refused!");
        qWarning() << "Error in JSON recieved" << '\n' << QString(error.c_str());
    }

    if(data["Type"].string_value() == "Login_success") {
        if(data["Success"].bool_value() == true) {
            gamestruct::set_self(data);
            dynamic_cast<Window *> (this->parentWidget())->setLoadingUi();
        }
        else
            popupBox("Login failed!");
    }
}

void LoginWidget::popupBox(const QString message) {
    QMessageBox::information(this, "", message);
}
