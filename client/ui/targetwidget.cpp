#include "targetwidget.h"
#include "playerwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"

#include <QWidget>

using namespace json11;

TargetWidget::TargetWidget(QWidget *parent) : PlayerWidget(parent) {
    QSizePolicy sp_retain = this->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    this->setSizePolicy(sp_retain);
}

const Player & TargetWidget::target() const {
    return _target;
}

void TargetWidget::check_target(const std::vector<Player> & vec) {
    auto it = std::find(vec.begin(), vec.end(), _target);
    if(it != vec.end()) {
        update_target(*it);
    } else {
        unselect_target();
    }
}

void TargetWidget::select_target(const Player & p, bool combat) {
    update_target(p);

    if(combat) {
        attack_mutex.lock();
        const Json data = Json::object {
            {"Type", "Attack"},
            {"Condition", "Start"},
            {"Victim", p.name()}
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
}

void TargetWidget::update_target(const Player & p) {
    attack_mutex.lock();
    _target = p;
    attack_mutex.unlock();
    this->update();
}

void TargetWidget::stop_attack() {
    const Json data = Json::object {
        {"Type", "Attack"},
        {"Condition", "Stop"}
    };
    connection::output(data);
}

void TargetWidget::paintEvent(QPaintEvent * event) {
    paint(event, _target);
}
