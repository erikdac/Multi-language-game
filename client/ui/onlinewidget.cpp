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

using namespace json11;

OnlineWidget::OnlineWidget(QWidget * parent) : ui(new Ui::OnlineWidget) {
    this->setParent(parent);
    ui->setupUi(this);

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
    ScreenWidget * screenWidget = findChild<ScreenWidget *>("screenwidget");
    screenWidget->resume();
    connection::readAsync(this);
    setFocus();
}

void OnlineWidget::pause() {
    _movementController->clear();
    ScreenWidget * screenWidget = findChild<ScreenWidget*>();
    screenWidget->pause();
}

void OnlineWidget::input(std::string input) {
    std::string error;
    Json data = Json::parse(input, error);

    // TODO: Remove debug prints
    if(error.size() > 0) {
        std::cout << "ONLINEWIDGET ERROR: " << error << std::endl;
    }
    else {
        std::cout << "ONLINEWIDGET: " << data.dump() << std::endl;
    }

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
    std::cout << "BEFORE" << std::endl;
    connection::disconnect();
    std::cout << "AFTER" << std::endl;
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
