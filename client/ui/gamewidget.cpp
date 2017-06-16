#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "screenwidget.h"
#include "targetwidget.h"
#include "network/connection.h"
#include "window.h"
#include "game/entities/player.h"
#include "game/gamestruct.h"

#include <QtDebug>
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

    _isRunning = true;
}

GameWidget::~GameWidget() {
    _isRunning = false;
    connection::disconnect();
    gamestruct::clear();
    target_widget()->unselect_target();
    delete ui;
}

void GameWidget::resume() {
    qInfo("Resumed");
    player_widget()->setPlayer(gamestruct::self());
    target_widget()->setVisible(false);
    setFocus();
    _isRunning = true;
    connection::startReading();
}

void GameWidget::pause() {
    qInfo("Paused");
    _isRunning = false;
    connection::disconnect();
    _movementController.clear();
}

void GameWidget::tick(float deltaTime) {
    if (_isRunning) {
        processMouse();
        processKeyboard();

        if (_isRunning) {
            assert(_isRunning);
            processNetwork();
        }
        if (_isRunning) {
            assert(_isRunning);
            _movementController.execute();
            gamestruct::update_entities(deltaTime);
            _screenWidget->refresh();
        }
    }
}

void GameWidget::processMouse() {
    for (const QMouseEvent * e : _mouseHandler.events()) {
        const double x = gamestruct::self()->x() - (graphics::VIEW_WIDTH - 0.5) + (e->x() / (_screenWidget->width() / (graphics::VIEW_WIDTH * 2)));
        const double y = gamestruct::self()->y() - (graphics::VIEW_HEIGHT - 0.5) + (e->y() / (_screenWidget->height() / (graphics::VIEW_HEIGHT * 2)));
        Actor * actor = gamestruct::actor_at_position(x, y);
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
                _movementController.pushed('w');
            } else {
                _movementController.released('w');
            }
        }
        else if (e.first.key() == Qt::Key_A) {
            if (e.second) {
                _movementController.pushed('a');
            } else {
                _movementController.released('a');
            }
        }
        else if (e.first.key() == Qt::Key_S) {
            if (e.second) {
                _movementController.pushed('s');
            } else {
                _movementController.released('s');
            }
        }
        else if (e.first.key() == Qt::Key_D) {
            if (e.second) {
                _movementController.pushed('d');
            } else {
                _movementController.released('d');
            }
        }
        else if (e.first.key() == Qt::Key_Escape && e.second) {
            openMenu();
        }
    }
}

void GameWidget::processNetwork() {

    for (const Json & data : connection::_inputHandler.events()) {

        const std::string type = data["Type"].string_value();
        if (type == "Disconnect") {
            logout();
            break;
        }
        else if (type == "Map") {
            gamestruct::new_map(data, target_widget());
        }
        else if (type == "Player_update") {
            gamestruct::update_player(data, target_widget());
        }
        else if (type == "Creature_update") {
            gamestruct::update_creature(data, target_widget());
        }
        else if (type == "Actor_removed") {
            gamestruct::remove_actor(data, target_widget());
        }

        // SELF

        else if (type == "Moved") {
            gamestruct::self()->fix_position(data["NewX"].number_value(), data["NewY"].number_value());
        }
        else if (type == "Attacked") {
            gamestruct::self()->set_health(data["Health"].number_value());
            player_widget()->update();
        }

        // Unknown packet
        else {
            qWarning("Unknown JSON Type recieved!");
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
