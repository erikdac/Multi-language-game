#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "onlinewidget.h"
#include "network/connection.h"
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"
#include "game/map.h"
#include "game/objects/graphics.h"

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

    // Self
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
        glVertex2f(-0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
    glEnd();

    others_mutex.lock();
    for(unsigned int i = 0; i < _other_players.size(); ++i) {
        _other_players[i].load_graphics();
    }
    others_mutex.unlock();

    float x = (0.5f + 105.0f - _player->x())/VIEW_WIDTH;
    float y = -(0.5f + 105.0f - _player->y())/VIEW_HEIGHT;

    // Tree
    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();

    x = (0.5f + 95.0f - _player->x())/VIEW_WIDTH;
    y = -(0.5f + 95.0f - _player->y())/VIEW_HEIGHT;

    // Tree
    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();
}

void GameWidget::resizeGL() {

}

void GameWidget::stop_refreshing() {
    _screenRefresher->stop();
}

void GameWidget::mousePressEvent(QMouseEvent *event) {
    unsigned int x = _player->x() - VIEW_WIDTH + event->x() / (this->width() / (VIEW_WIDTH * 2 + 1));
    unsigned int y = _player->y() - VIEW_HEIGHT + event->y() / (this->height() / (VIEW_HEIGHT * 2 + 1));
    Player * player = map::player_at_position(x, y);
    OnlineWidget * w = dynamic_cast<OnlineWidget *> (this->parentWidget());
    w->switch_target(player);
}
