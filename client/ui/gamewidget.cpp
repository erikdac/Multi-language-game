#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "onlinewidget.h"
#include "network/connection.h"
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"
#include "game/map.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>
#include <QMouseEvent>

GameWidget::GameWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    setScreenRefresher();
}

GameWidget::~GameWidget() {
    _screenRefresher->stop();
    delete ui;
}

void GameWidget::setScreenRefresher() {
    _screenRefresher = new ScreenRefresher();
    QObject::connect(
        _screenRefresher, SIGNAL(repaint()), this, SLOT(repaint())
    );
    _screenRefresher->start();
}

void GameWidget::intitializeGL() {
    glClearColor(255, 255, 255, 1);
}

void GameWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const Environment * e : _environment) {
        e->load_graphics();
    }

    others_mutex.lock();
    for (const Player & p : _other_players) {
        p.load_graphics();
    }
    others_mutex.unlock();

    _self->load_graphics();
}

void GameWidget::resizeGL() {

}

void GameWidget::stop_refreshing() {
    _screenRefresher->stop();
}

void GameWidget::mousePressEvent(QMouseEvent * event) {
    unsigned int x = _self->x() - VIEW_WIDTH + event->x() / (this->width() / (VIEW_WIDTH * 2 + 1));
    unsigned int y = _self->y() - VIEW_HEIGHT + event->y() / (this->height() / (VIEW_HEIGHT * 2 + 1));
    Player * player = map::player_at_position(x, y);
    OnlineWidget * w = dynamic_cast<OnlineWidget *> (this->parentWidget());
    w->switch_target(player);
}
