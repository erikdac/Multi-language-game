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
#include <QMouseEvent>
#include <utility>

using namespace json11;

OnlineWidget::OnlineWidget(QWidget * parent) : ui(new Ui::OnlineWidget) {
    this->setParent(parent);
    ui->setupUi(this);

    _screenWidget = findChild<ScreenWidget *>("screenwidget");
    _screenWidget->setMouseHandler(&_mouseHandler);
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
    setFocus();
}

void OnlineWidget::pause() {
    _movementController->clear();
}

void OnlineWidget::process() {
    processMouse();
    processKeyboard();
    _movementController->execute();
    processNetwork();
    _screenWidget->repaint();
}

void OnlineWidget::processNetwork() {
    std::string packet = connection::readPacket(1);
    if (packet.empty()) {
        return;
    }
    std::cout << packet << std::endl;

    std::string error;
    Json data = Json::parse(packet, error);

    const std::string type = data["Type"].string_value();
    if (type == "Disconnect") {
        logout();
    }
    else if (type == "Map") {
        map::parse_map(data);
        target_widget()->check_target(_other_players);
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

void OnlineWidget::processMouse() {
    std::vector<QMouseEvent *> mouseEvents = _mouseHandler.events();

    for (const QMouseEvent * e : mouseEvents) {
        unsigned int x = _self->x() - VIEW_WIDTH + e->x() / (_screenWidget->width() / (VIEW_WIDTH * 2 + 1));
        unsigned int y = _self->y() - VIEW_HEIGHT + e->y() / (_screenWidget->height() / (VIEW_HEIGHT * 2 + 1));
        Player * player = map::player_at_position(x, y);
        if(player) {
            target_widget()->select_target(*player, true);
        }
        else {
            target_widget()->unselect_target();
        }
    }
}

typedef std::pair<QKeyEvent, bool> key_pair;

void OnlineWidget::processKeyboard() {
    std::vector<key_pair> keyEvents = _keyboardHandler.events();

    for (const key_pair & e : keyEvents) {

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

void OnlineWidget::logout() {
    connection::disconnect();
    dynamic_cast<Window *> (this->parentWidget())->setLoginUi();
}

void OnlineWidget::keyPressEvent(QKeyEvent * event) {
    if (!event->isAutoRepeat()) {
        std::pair<QKeyEvent, bool> p(*event, true);
        _keyboardHandler.addEvent(p);
    }
}

void OnlineWidget::keyReleaseEvent(QKeyEvent * event) {
    if (!event->isAutoRepeat()) {
        std::pair<QKeyEvent, bool> p(*event, false);
        _keyboardHandler.addEvent(p);
    }
}

// TODO: Implement a game menu instead of just logging out.
void OnlineWidget::openMenu() {
    logout();
}

TargetWidget * OnlineWidget::target_widget() const {
    return findChild<TargetWidget *>("targetwidget");
}
