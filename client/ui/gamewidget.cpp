#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "network/connection.h"
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"
#include "game/map.h"
#include "game/objects/graphics.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

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

    Graphics * g = _player;
    g->load_graphics();

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
}

void GameWidget::resizeGL() {

}

void GameWidget::stop_refreshing() {
    _screenRefresher->stop();
}

