#include "gamewidget.h"
#include "ui_gamewidget.h"
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
#include <QMouseEvent>
#include <utility>
#include <cassert>
#include <thread>

using namespace json11;

GameWidget::GameWidget(QWidget * parent) : ui(new Ui::GameWidget) {
    this->setParent(parent);
    ui->setupUi(this);

    _screenWidget = findChild<ScreenWidget *>("screenwidget");
    _screenWidget->setMouseHandler(&_mouseHandler);
    _movementController = new MovementController();

    _isRunning = true;
}

GameWidget::~GameWidget() {
    _isRunning = false;
    map::cleanMap();
    delete _movementController;
    delete ui;
}

void GameWidget::resume() {
    player_widget()->setPlayer(_self);
    target_widget()->setVisible(false);
    setFocus();
    _isRunning = true;
    std::thread(&GameWidget::networkReader, this).detach();
}

void GameWidget::pause() {
    _isRunning = false;
    _movementController->clear();
}

void GameWidget::process() {
    if (_isRunning) {
        processMouse();
        processKeyboard();

        if (_isRunning) {
            assert(_isRunning);
            processNetwork();
        }
        if (_isRunning) {
            assert(_isRunning);
            _movementController->execute();
            _screenWidget->refresh();
        }
    }
}

void GameWidget::processMouse() {
    for (const QMouseEvent * e : _mouseHandler.events()) {
        const double x = _self->x() - (VIEW_WIDTH - 0.5) + (e->x() / (_screenWidget->width() / (VIEW_WIDTH * 2)));
        const double y = _self->y() - (VIEW_HEIGHT - 0.5) + (e->y() / (_screenWidget->height() / (VIEW_HEIGHT * 2)));
        Actor * actor = map::actor_at_position(x, y);
        if(actor) {
            target_widget()->select_target(actor);
        }
        else {
            target_widget()->unselect_target();
        }
    }
}

typedef std::pair<QKeyEvent, bool> key_pair;

void GameWidget::processKeyboard() {
    for (const key_pair & e : _keyboardHandler.events()) {

        if (e.first.key() == Qt::Key_W) {
            if (e.second) {
                _movementController->pushed('w');
            } else {
                _movementController->released('w');
            }
        }
        else if (e.first.key() == Qt::Key_A) {
            if (e.second) {
                _movementController->pushed('a');
            } else {
                _movementController->released('a');
            }
        }
        else if (e.first.key() == Qt::Key_S) {
            if (e.second) {
                _movementController->pushed('s');
            } else {
                _movementController->released('s');
            }
        }
        else if (e.first.key() == Qt::Key_D) {
            if (e.second) {
                _movementController->pushed('d');
            } else {
                _movementController->released('d');
            }
        }
        else if (e.first.key() == Qt::Key_Escape && e.second) {
            openMenu();
        }
    }
}

void GameWidget::processNetwork() {

    for (const Json & data : _networkHandler.events()) {

        const std::string type = data["Type"].string_value();
        if (type == "Disconnect") {
            logout();
            break;
        }
        else if (type == "Map") {
            map::parse_map(data, target_widget());
        }
        else if (type == "Player_update") {
            map::update_player(data, target_widget());
        }
        else if (type == "Creature_update") {
            map::update_creature(data, target_widget());
        }
        else if (type == "Actor_removed") {
            map::remove_actor(data, target_widget());
        }

        // SELF

        else if (type == "Moved") {
            _self->set_position(data["NewX"].number_value(), data["NewY"].number_value());
        }
        else if (type == "Attacked") {
            _self->set_health(data["Health"].number_value());
            player_widget()->update();
        }
    }
}

// TODO: Implement a game menu instead of just logging out.
void GameWidget::openMenu() {
    logout();
}

void GameWidget::logout() {
    _isRunning = false;
    connection::disconnect();
    dynamic_cast<Window *> (this->parentWidget())->setLoginUi();
    assert(!_isRunning);
}

void GameWidget::keyPressEvent(QKeyEvent * event) {
    if (!event->isAutoRepeat()) {
        std::pair<QKeyEvent, bool> p(*event, true);
        _keyboardHandler.addEvent(p);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent * event) {
    if (!event->isAutoRepeat()) {
        std::pair<QKeyEvent, bool> p(*event, false);
        _keyboardHandler.addEvent(p);
    }
}

void GameWidget::networkReader() {
    while (_isRunning) {
        std::string packet = connection::readPacket(10);
        if (!packet.empty()) {
            std::string error;
            Json data = Json::parse(packet, error);
            if (error.empty()) {
                _networkHandler.addEvent(data);
            } else if (_isRunning) {
                std::string error = "\tError in JSON recieved: " + packet;
                std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
                std::cerr << error << std::endl;

                data = Json::object {
                    {"Type", "Disconnect"},
                };
                _networkHandler.addEvent(data);
                return;
            }
        }
    }
    assert(!_isRunning);
}

PlayerWidget * GameWidget::player_widget() const {
    auto playerWidget = findChild<PlayerWidget *>("playerwidget");
    assert(playerWidget != 0);
    return playerWidget;
}

TargetWidget * GameWidget::target_widget() const {
    auto targetWidget = findChild<TargetWidget *>("targetwidget");
    assert(targetWidget != 0);
    return targetWidget;
}
