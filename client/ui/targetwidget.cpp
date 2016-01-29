#include "targetwidget.h"
#include "ui_targetwidget.h"
#include "playerwidget.h"

#include <iostream>
#include <QWidget>

TargetWidget::TargetWidget(QWidget *parent) : PlayerWidget(parent) {

}

Player * TargetWidget::target() const {
    return _player;
}

void TargetWidget::select_target(Player * player) {
    _player = player;
    this->setVisible(true);
    update();
}

void TargetWidget::unselect_target() {
    this->setVisible(false);
    _player = 0;
}
