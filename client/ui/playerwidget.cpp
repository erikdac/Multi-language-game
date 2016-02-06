#include "ui_playerwidget.h"
#include "playerwidget.h"
#include "game/map.h"

#include <QLabel>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <QSizePolicy>

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

void PlayerWidget::paintEvent(QPaintEvent * event) {
    QRect rect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect.width() * 0.1, 14, QString::fromStdString(_player->name()));

    painter.fillRect(rect.width() * 0.05, 23, 300, 12, Qt::red);
    painter.drawRect(rect.width() * 0.05, 23, rect.width()*0.9, 12);

    painter.fillRect(rect.width() * 0.05, 43, 300, 12, Qt::blue);
    painter.drawRect(rect.width() * 0.05, 43, rect.width()*0.9, 12);
}
