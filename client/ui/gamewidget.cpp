#include "ui_gamewidget.h"
#include "gamewidget.h"
#include "mainwindow.h"
#include "network/connection.h"
#include "game/player.h"

#include <QKeyEvent>

Player * player;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    setFocus();

    player = new Player(100, 100, 100, 0);
}

GameWidget::~GameWidget() {
    delete ui;
}

void GameWidget::input(std::string input) {

}

void GameWidget::keyPressEvent(QKeyEvent *event) {

    if(event->key() == Qt::Key_W) {
        player->moveForward();
    } else if(event->key() == Qt::Key_S) {
        player->moveBackward();
    } else if(event->key() == Qt::Key_A) {
        player->turnLeft();
    } else if(event->key() == Qt::Key_D) {
        player->turnRight();
    } else if(event->key() == Qt::Key_Escape) {
        openMenu();
    }
    player->printForTest();
}

// TODO: Implement a game menu instead of just logging out.
void GameWidget::openMenu() {
    connection::disconnect();
    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();
}
