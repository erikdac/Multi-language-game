#include "ui_playerwidget.h"
#include "playerwidget.h"
#include "game/map.h"

#include <QLabel>

PlayerWidget::PlayerWidget(QWidget *parent, Player * player) :
    QWidget(parent),
    ui(new Ui::PlayerWidget),
    _player(player)
{
    ui->setupUi(this);
}

PlayerWidget::~PlayerWidget() {
    delete ui;
}

void PlayerWidget::update() {
    findChild<QLabel *>("Name")->setText(QString::fromStdString(_player->name()));
}
