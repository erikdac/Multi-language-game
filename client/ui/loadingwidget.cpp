#include "loadingwidget.h"
#include "ui_loadingwidget.h"
#include "window.h"
#include "network/connection.h"
#include "external/json11/json11.hpp"
#include "game/gamestruct.h"

#include <QtDebug>
#include <chrono>

using namespace json11;

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
    _hasSelf = false;
    _hasMap = false;
}

void LoadingWidget::tick(float deltaTime) {
    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    while ((!_hasSelf || !_hasMap) && delay < 30000) {
        std::vector<Json> input = connection::read();
        for (const Json & packet : input) {
            parsePacket(packet);
        }
        end = std::chrono::high_resolution_clock::now();
        delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
    if (_hasSelf && _hasMap) {
        dynamic_cast<Window *> (this->parentWidget())->setGameUi();
    } else {
        dynamic_cast<Window *> (this->parentWidget())->setLoginUi();
    }
}

void LoadingWidget::parsePacket(const Json & packet) {
    const std::string type = packet["Type"].string_value();
    if(type == "Self") {
        qDebug() << "Self player recieved!";
        gamestruct::set_self(packet);
        _hasSelf = true;
    } else if (type == "Map") {
        qDebug() << "Init map recieved!";
        gamestruct::init_map(packet);
        _hasMap = true;
    }
}
