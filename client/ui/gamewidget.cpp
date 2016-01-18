#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "network/connection.h"
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"
#include "game/map.h"
#include "game/objects/graphics.h"

#include <QKeyEvent>
#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

GameWidget::GameWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , ui(new Ui::GameWidget)
{
    initializeMap();

    ui->setupUi(this);
    setFocus();

    setScreenRefresher();
}

GameWidget::~GameWidget() {
    _screenRefresher->stop();
    delete ui;
}

void GameWidget::input(std::string input) {

}

void GameWidget::setScreenRefresher() {
    _screenRefresher = new ScreenRefresher();
    QObject::connect(
        _screenRefresher, SIGNAL(repaint()), this, SLOT(repaint())
    );
    _screenRefresher->start();
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        return;
    }

    switch(event->key()) {
        case Qt::Key_W:
            setKeyboardController('w');
        break;
        case Qt::Key_A:
            setKeyboardController('a');
        break;
        case Qt::Key_S:
            setKeyboardController('s');
        break;
        case Qt::Key_D:
            setKeyboardController('d');
        break;
        case Qt::Key_Escape:
            openMenu();
        break;
    }
}

void GameWidget::setKeyboardController(char key) {
    if(_keyMap.find(key) == _keyMap.end()) {
        KeyboardController * temp = new KeyboardController(key);
        _keyMap[key] = temp;
        temp->start();
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat() == false) {
        char c;
        switch(event->key()) {
            case Qt::Key_W: c = 'w'; break;
            case Qt::Key_A: c = 'a'; break;
            case Qt::Key_S: c = 's'; break;
            case Qt::Key_D: c = 'd'; break;
            default: return;
        }

        KeyboardController * temp = _keyMap[c];
        temp->stop();
        _keyMap.erase(c);
    }
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

// TODO: Implement a game menu instead of just logging out.
void GameWidget::openMenu() {
    connection::disconnect();
    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();
}
