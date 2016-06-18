#include "ui_playerwidget.h"
#include "playerwidget.h"
#include "game/map.h"

#include <QLabel>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <QSizePolicy>

PlayerWidget::PlayerWidget(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::PlayerWidget)
{
    ui->setupUi(this);
}

PlayerWidget::~PlayerWidget() {
    delete ui;
}

void PlayerWidget::setPlayer(Self * self) {
    _player = self;
}

void PlayerWidget::paintEvent(QPaintEvent * event) {
    paint(event, *_player);
}

void PlayerWidget::paint(QPaintEvent * event, Player & p) {
    QRect rect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect.width() * 0.08, 14, QString::fromStdString("(" + std::to_string(p.level()) + ") " + p.name()));

    int health_progress = (rect.width() * 0.9) * ((double)p.health() / p.max_health());
    painter.fillRect(rect.width() * 0.05, 23, health_progress, 12, Qt::red);
    painter.drawRect(rect.width() * 0.05, 23, (rect.width() * 0.9), 12);

    int mana_progress = (rect.width() * 0.9) * ((double)p.mana() / p.max_mana());
    painter.fillRect(rect.width() * 0.05, 43, mana_progress, 12, Qt::blue);
    painter.drawRect(rect.width() * 0.05, 43, (rect.width() * 0.9), 12);
}
