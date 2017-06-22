#include "loadingwidget.h"
#include "ui_loadingwidget.h"
#include "window.h"
#include "network/connection.h"
#include "external/json11/json11.hpp"
#include "game/gamestruct.h"

#include <QtDebug>

#include <thread>
#include <chrono>

LoadingWidget::LoadingWidget(QWidget *parent)
    : ui(new Ui::LoadingWidget)
{
    this->setParent(parent);
    ui->setupUi(this);
}

LoadingWidget::~LoadingWidget() {
    delete ui;
}

void LoadingWidget::resume() {
    qDebug("Resumed");
}

void LoadingWidget::pause() {
    qDebug("Paused");
}

void LoadingWidget::tick(float deltaTime) {
    // TODO: fix...
    std::vector<json11::Json> input;
    while (true) {
        qDebug() << "WAITING!";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        input = connection::read();
        if(input.front()["Type"].string_value() == "Self") {
            qDebug() << "Self player recieved!";
            gamestruct::set_self(input.front());
            break;
        }
    }
    dynamic_cast<Window *> (this->parentWidget())->setGameUi();
}

void LoadingWidget::parsePacket(const json11::Json & packet) {
    if(packet["Type"].string_value() == "Self") {
        qDebug() << "Self player recieved!";
        gamestruct::set_self(packet);
    }
}
