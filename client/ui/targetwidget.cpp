#include "targetwidget.h"
#include "ui_targetwidget.h"
#include "playerwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "game/map.h"

#include <QWidget>

using namespace json11;

TargetWidget::TargetWidget(QWidget *parent) : PlayerWidget(parent) {

}

Player * TargetWidget::target() const {
    return _player;
}

void TargetWidget::select_target(Player * player, bool combat) {
    update_target(player);

    if(combat) {
        const Json data = Json::object {
            {"Type", "Attack"},
            {"Condition", "Start"},
            {"Victim", _player->name()}
        };
        connection::output(data);
    } else {
        stop_attack();
    }

    this->setVisible(true);
    update();
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
