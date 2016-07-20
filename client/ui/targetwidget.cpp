#include "targetwidget.h"
#include "playerwidget.h"
#include "network/connection.h"
#include "json11/json11.hpp"

#include <iostream>
#include <QWidget>

using namespace json11;

TargetWidget::TargetWidget(QWidget *parent) : PlayerWidget(parent) {
    QSizePolicy sp_retain = this->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    this->setSizePolicy(sp_retain);
}

std::string TargetWidget::target() const {
    if (_target) {
        return _target->name();
    } else {
        return "";
    }
}

void TargetWidget::check_target(const std::vector<Player *> & vec) {
    auto it = std::find_if(vec.begin(), vec.end(), [=](const Player * a) {return a->name() == target();});
    if(it != vec.end()) {
        update_target(*it);
    } else {
        unselect_target();
    }
}

void TargetWidget::select_target(Actor * a, bool combat) {
    update_target(a);

    if(combat) {
        const Json data = Json::object {
            {"Type", "Attack"},
            {"Condition", "Start"},
            {"Victim", a->name()}
        };
        connection::output(data);
    } else {
        stop_attack();
    }

    this->setVisible(true);
}

void TargetWidget::unselect_target() {
    this->setVisible(false);
    stop_attack();
    _target = 0;
}

void TargetWidget::update_target(Actor * a) {
    _target = a;
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
