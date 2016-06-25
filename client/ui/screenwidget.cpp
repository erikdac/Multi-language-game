#include "ui_screenwidget.h"
#include "screenwidget.h"
#include "window.h"
#include "onlinewidget.h"
#include "network/connection.h"
#include "game/objects/player.h"
#include "game/map.h"

#include <iostream>
#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>
#include <QMouseEvent>
#include <thread>
#include <chrono>
#include "unistd.h"

static const unsigned int MAX_FPS = 30;

ScreenWidget::ScreenWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::ScreenWidget)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(repaint()), this, SLOT(repaint()));
}

ScreenWidget::~ScreenWidget() {
    delete ui;
}

void ScreenWidget::intitializeGL() {
    glClearColor(255, 255, 255, 1);
}

void ScreenWidget::mousePressEvent(QMouseEvent * event) {
    unsigned int x = _self->x() - VIEW_WIDTH + event->x() / (this->width() / (VIEW_WIDTH * 2 + 1));
    unsigned int y = _self->y() - VIEW_HEIGHT + event->y() / (this->height() / (VIEW_HEIGHT * 2 + 1));
    Player * player = map::player_at_position(x, y);
    OnlineWidget * w = dynamic_cast<OnlineWidget *> (this->parentWidget());
    w->switch_target(player);
}

void ScreenWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    environment_mutex.lock();
    for (const Environment * e : _environment) {
        e->load_graphics();
    }
    environment_mutex.unlock();

    others_mutex.lock();
    for (const Player & p : _other_players) {
        p.load_graphics();
    }
    others_mutex.unlock();

    _self->load_graphics();
}

