#include "onlinewidget.h"
#include "ui_onlinewidget.h"
#include "screenwidget.h"
#include "targetwidget.h"
#include "network/connection.h"
#include "window.h"
#include "game/objects/player.h"
#include "game/movementcontroller.h"
#include "game/map.h"
#include "game/objects/graphics.h"

#include <iostream>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>
#include <chrono>
#include <unistd.h>

using namespace json11;

static const unsigned int MAX_FPS = 20;

OnlineWidget::OnlineWidget(QWidget * parent) : ui(new Ui::OnlineWidget) {
    this->setParent(parent);
    ui->setupUi(this);

    _screenWidget = findChild<ScreenWidget *>("screenwidget");
    _movementController = new MovementController();
}

OnlineWidget::~OnlineWidget() {
    map::cleanMap();
    delete _movementController;
    delete ui;
}

void OnlineWidget::resume() {
    findChild<PlayerWidget *>("playerwidget")->setPlayer(_self);
    target_widget()->setVisible(false);
    std::thread(&OnlineWidget::gameLoop, this).detach();
    setFocus();
}

void OnlineWidget::pause() {
    _movementController->clear();
    _isRunning = false;
    while (!_isFinished) {
        usleep(1000);
    }
}

void OnlineWidget::gameLoop() {
    _isRunning = true;
    _isFinished = false;
    while(_isRunning) {
        auto begin = std::chrono::high_resolution_clock::now();
        process();
        auto end = std::chrono::high_resolution_clock::now();
        int diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        usleep(((1000/MAX_FPS) - diff) * 1000);
    }
    _isFinished = true;
}

void OnlineWidget::process() {
    processNetwork();
    _screenWidget->repaint();
}

void OnlineWidget::processNetwork() {
    std::string packet = connection::readPacket(5);
    if (packet.empty()) {
        return;
    }

    std::string error;
    Json data = Json::parse(packet, error);

    const std::string type = data["Type"].string_value();
    if (type == "Disconnect") {
        logout();
    }
    else if (type == "Map") {
        map::parse_map(data);
        others_mutex.lock();
        target_widget()->check_target(_other_players);
        others_mutex.unlock();
    }
    else if (type == "Player_update") {
        map::update_player(data, target_widget());
    }
    else if (type == "Player_removed") {
        map::remove_player(data, target_widget());
    }

    // SELF

    else if (type == "Moved") {
        _self->update_position(data["NewX"].number_value(), data["NewY"].number_value());
    }
    else if (type == "Attacked") {
        _self->update_health(data["Health"].number_value());
        findChild<PlayerWidget *>("playerwidget")->update();
    }
}

void OnlineWidget::logout() {
    pause();
    map::cleanMap();
    connection::disconnect();
    Window * w = dynamic_cast<Window *> (this->parentWidget());
    w->setLoginUi();
}

void OnlineWidget::keyPressEvent(QKeyEvent *event) {

    if(event->isAutoRepeat()) {
        return;
    }

    switch(event->key()) {
        case Qt::Key_W:
            _movementController->pushed('w');
        break;
        case Qt::Key_A:
            _movementController->pushed('a');
        break;
        case Qt::Key_S:
            _movementController->pushed('s');
        break;
        case Qt::Key_D:
            _movementController->pushed('d');
        break;
        case Qt::Key_Escape:
            openMenu();
        break;
    }
}

void OnlineWidget::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat() == false) {
        char c;
        switch(event->key()) {
            case Qt::Key_W: c = 'w'; break;
            case Qt::Key_A: c = 'a'; break;
            case Qt::Key_S: c = 's'; break;
            case Qt::Key_D: c = 'd'; break;
            default: return;
        }

        if(c == 'w' || c == 'a' || c == 's' || c == 'd') {
            _movementController->released(c);
        }
    }
}

// TODO: Implement a game menu instead of just logging out.
void OnlineWidget::openMenu() {
    logout();
}

TargetWidget * OnlineWidget::target_widget() const {
    return findChild<TargetWidget *>("targetwidget");
}

void OnlineWidget::switch_target(Player * player) {
    if(player) {
        target_widget()->select_target(*player, true);
    }
    else {
        target_widget()->unselect_target();
    }
}
