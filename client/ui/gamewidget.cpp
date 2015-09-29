#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "network/connection.h"
#include "game/player.h"
#include "game/keyboardcontroller.h"

#include <QKeyEvent>
#include <QPainter>

KeyboardController * test;
bool running = false;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    setFocus();

    // TODO: Remove
    _player = new Player(100, 100, 100, 0);
    test = new KeyboardController(_player);
}

GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::input(std::string input) {

}

void GameWidget::animate() {
    repaint();
}


void GameWidget::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) {
        return;
    }

    if(event->key() == Qt::Key_W) {
        _player->moveForward();
    } else if(event->key() == Qt::Key_S) {
        _player->moveBackward();
    } else if(event->key() == Qt::Key_A) {
        _player->turnLeft();
    } else if(event->key() == Qt::Key_D) {
        _player->turnRight();
    } else if(event->key() == Qt::Key_Escape) {
        openMenu();
    }

    if(!running) {
        QObject::connect(
                    test, SIGNAL(animate()), this, SLOT(animate())
                );
        test->start();
        _keyMap['a'] = "yay";
        running = true;
    }

//    player->printForTest();
    repaint();
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat() == false) {
        test->stop();
        running = false;
    }
}

void GameWidget::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPainterPath painterPath;
    QPen pen;
    pen.setWidth(8);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    QBrush color;
    color.setStyle(Qt::SolidPattern);

    QPolygon polygon;
    polygon << QPoint(_player->_x+10, _player->_y+10);
    polygon << QPoint(_player->_x+10, _player->_y+100);
    polygon << QPoint(_player->_x+100, _player->_y+100);
    polygon << QPoint(_player->_x+100, _player->_y+10);
    painter.drawPolygon(polygon);

    painterPath.addPolygon(polygon);
    color.setColor(Qt::green);
    painter.fillPath(painterPath, color);
}

// TODO: Implement a game menu instead of just logging out.
void GameWidget::openMenu() {
    connection::disconnect();
    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();
}
