#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "network/connection.h"
#include "game/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"
#include "game/map.h"

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
    _screenRefresher->terminate();
    delete ui;
}

void GameWidget::input(std::string input) {

}

void GameWidget::animate() {
    repaint();
}

void GameWidget::setScreenRefresher() {
    _screenRefresher = new ScreenRefresher();
    QObject::connect(
        _screenRefresher, SIGNAL(animate()), this, SLOT(animate())
    );
    _screenRefresher->start();
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        return;
    }

    if(event->key() == Qt::Key_W) {
        setKeyboardController('w');
    } else if(event->key() == Qt::Key_S) {
        setKeyboardController('s');
    } else if(event->key() == Qt::Key_A) {
        setKeyboardController('a');
    } else if(event->key() == Qt::Key_D) {
        setKeyboardController('d');
    } else if(event->key() == Qt::Key_Escape) {
        openMenu();
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
        if(event->key() == Qt::Key_W) {
            KeyboardController * temp = _keyMap['w'];
            temp->terminate();
            _keyMap.erase('w');
        } else if(event->key() == Qt::Key_S) {
            KeyboardController * temp = _keyMap['s'];
            temp->terminate();
            _keyMap.erase('s');
        } else if(event->key() == Qt::Key_A) {
            KeyboardController * temp = _keyMap['a'];
            temp->terminate();
            _keyMap.erase('a');
        } else if(event->key() == Qt::Key_D) {
            KeyboardController * temp = _keyMap['d'];
            temp->terminate();
            _keyMap.erase('d');
        }
    }
}

void GameWidget::intitializeGL() {
    glClearColor(255, 255, 255, 1);
}

void GameWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Player
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
        glVertex2f(-0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
    glEnd();

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
