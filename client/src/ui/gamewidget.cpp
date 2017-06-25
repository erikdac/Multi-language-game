#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "screenwidget.h"
#include "targetwidget.h"
#include "network/connection.h"
#include "window.h"
#include "game/entities/player.h"
#include "game/gamestruct.h"
#include "config.h"

#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>
#include <chrono>
#include <unistd.h>
#include <QMouseEvent>
#include <utility>
#include <cassert>

using namespace json11;

GameWidget::GameWidget(QWidget * parent)
    : ui(new Ui::GameWidget)
    , _movementController(MovementController(MOVEMENT_DELAY_MILLIS))
{

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
}

void GameWidget::pause() {
    qInfo("Paused");
    _isRunning = false;
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
            _movementController.execute(gamestruct::self());
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
        else if (target_widget()->target() != ""){
            target_widget()->unselect_target();
        }
    }
}

void GameWidget::processKeyboard() {
    for (const std::pair<QKeyEvent, bool> & e : _keyboardHandler.events()) {
        const int key = e.first.key();
        if (key == Qt::Key_W || key == Qt::Key_A || key == Qt::Key_S || key == Qt::Key_D) {
            if (e.second) {
                _movementController.pushed(key);
            } else {
                _movementController.released(key);
            }
        }
        else if (key == Qt::Key_Escape && e.second) {
            openMenu();
        }
    }
}

void GameWidget::processNetwork() {
    for (const Json & data : connection::read()) {
        const std::string type = data["Type"].string_value();
        if (type == "Disconnect") {
            qDebug() << "Disconnect network packet recieved!";
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
            qWarning() << "Unknown JSON Type recieved!";
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
