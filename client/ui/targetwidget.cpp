#include "targetwidget.h"
#include "ui_targetwidget.h"
#include "playerwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"
#include "game/map.h"

#include <QWidget>

using namespace json11;

TargetWidget::TargetWidget(QWidget *parent) : PlayerWidget(parent), _attack(this) {

}

Player * TargetWidget::target() const {
    return _player;
}

TargetWidget::~TargetWidget() {
    _combat = false;
    _attack.wait();
}

void TargetWidget::select_target(Player * player, bool combat) {
    update_target(player);
    _combat = combat;
    if(_combat && _attack.isRunning() == false) {
        _attack.start();
    }
    this->setVisible(true);
    update();
}

void TargetWidget::unselect_target() {
    this->setVisible(false);
    _combat = false;
    _attack.wait();
    _player = 0;
}

void TargetWidget::update_target(Player * player) {
    _attack.attack_mutex.lock();
    _player = player;
    _attack.attack_mutex.unlock();
}

TargetWidget::Attack::Attack(TargetWidget * const outer) : _outer(outer) {

}

/********************************************************************
 * The inner thread for auto-attacking
 *******************************************************************/

void TargetWidget::Attack::run() {
    safe_sleep(ATTACK_DELAY_MS);

    while(_outer->_combat) {
        if(_self->distance_to_player(_outer->_player) <= 1) {
            hit();
            safe_sleep(ATTACK_DELAY_MS);
        }
        else {
            safe_sleep(100);
        }
    }
}

void TargetWidget::Attack::hit() {
    attack_mutex.lock();
    const Json data = Json::object {
        {"Type", "Attack"},
        {"Victim", _outer->_player->name()}
    };
    attack_mutex.unlock();
    connection::output(data);
}

void TargetWidget::Attack::safe_sleep(const unsigned int ms) {
    for(unsigned int i = 0; i < ms && _outer->_combat; ++i)
        usleep(1000);
}
