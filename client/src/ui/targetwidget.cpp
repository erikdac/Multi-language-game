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

    QObject::connect(
        this, SIGNAL(activate(bool)),
        this, SLOT(setVisible(bool))
    );
}

std::string TargetWidget::target() const {
    if (_target) {
        return _target->name();
    } else {
        return "";
    }
}

void TargetWidget::select_target(Actor * a) {
    update_target(a);
    emit activate(true);
    const Json data = Json::object {
        {"Type", "Attack"},
        {"Condition", "Start"},
        {"Victim", a->name()}
    };
    connection::write(data);
}

void TargetWidget::unselect_target() {
    emit activate(false);
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
    connection::write(data);
}

void TargetWidget::paintEvent(QPaintEvent * event) {
    paint(event, _target);
}
