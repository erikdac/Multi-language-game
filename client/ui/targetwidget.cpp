#include "targetwidget.h"
#include "ui_targetwidget.h"
#include "playerwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "game/map.h"

#include <QWidget>

using namespace json11;

TargetWidget::TargetWidget(QWidget *parent) : PlayerWidget(parent) {
    QSizePolicy sp_retain = this->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    this->setSizePolicy(sp_retain);
}

std::string TargetWidget::target() const {
    if(_player == 0) {
        return "";
    }
    else {
        return _player->name();
    }
}

void TargetWidget::select_target(Player * player, bool combat) {
    update_target(player);

    if(combat) {
        attack_mutex.lock();
        const Json data = Json::object {
            {"Type", "Attack"},
            {"Condition", "Start"},
            {"Victim", _player->name()}
        };
        attack_mutex.unlock();
        connection::output(data);
    } else {
        stop_attack();
    }

    this->setVisible(true);
}

void TargetWidget::unselect_target() {
    this->setVisible(false);
    stop_attack();
    _player = 0;
}

void TargetWidget::update_target(Player * player) {
    attack_mutex.lock();
    _player = player;
    attack_mutex.unlock();
}

void TargetWidget::stop_attack() {
    const Json data = Json::object {
        {"Type", "Attack"},
        {"Condition", "Stop"}
    };
    connection::output(data);
}
