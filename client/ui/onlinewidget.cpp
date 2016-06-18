#include "onlinewidget.h"
#include "ui_onlinewidget.h"
#include "gamewidget.h"
#include "targetwidget.h"
#include "network/connection.h"
#include "mainwindow.h"
#include "game/objects/player.h"
#include "game/movementcontroller.h"
#include "game/map.h"
#include "game/objects/graphics.h"

#include <iostream>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>

using namespace json11;

OnlineWidget::OnlineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineWidget)
{

    ui->setupUi(this);

   PlayerWidget * playerWidget = new PlayerWidget(this);
    _player_widget = playerWidget;

    TargetWidget * targetWidget = new TargetWidget(this);
    _target_widget = targetWidget;
    targetWidget->setVisible(false);

    QWidget * bar = findChild<QWidget *> ("Bar");
    bar->layout()->addWidget(playerWidget);
    bar->layout()->addWidget(targetWidget);

    QGridLayout * gameLayout = findChild<QGridLayout *>("GameLayout");
    gameLayout->addWidget(new GameWidget(this));
}

void OnlineWidget::start() {
    _player_widget->setPlayer(_self);
    GameWidget * gameWidget = findChild<GameWidget *>("GameWidget");
    gameWidget->start_refreshing();
    connection::readAsync(this);
    setFocus();
}

OnlineWidget::~OnlineWidget() {
    delete ui;
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
    }
    else if (type == "Player_update") {
        map::update_player(data);
    }
    else if (type == "Player_removed") {
        map::remove_player(data);
    }

    // SELF

    else if (type == "Moved") {
        _self->update_position(data["NewX"].number_value(), data["NewY"].number_value());
    }
    else if (type == "Attacked") {
        _self->update_health(data["Health"].number_value());
        _player_widget->update();
    }
}

void OnlineWidget::logout() {
    GameWidget * gameWidget = findChild<GameWidget*>();
    gameWidget->stop_refreshing();
    connection::disconnect();
    MainWindow *w = dynamic_cast<MainWindow *> (this->parentWidget());
    w->setUpLoginUi();
}

void OnlineWidget::keyPressEvent(QKeyEvent *event) {

    if(event->isAutoRepeat()) {
        return;
    }

    switch(event->key()) {
        case Qt::Key_W:
            movement_controller::pushed('w');
        break;
        case Qt::Key_A:
            movement_controller::pushed('a');
        break;
        case Qt::Key_S:
            movement_controller::pushed('s');
        break;
        case Qt::Key_D:
            movement_controller::pushed('d');
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
            movement_controller::released(c);
        }
    }
}

// TODO: Implement a game menu instead of just logging out.
void OnlineWidget::openMenu() {
    logout();
}

void OnlineWidget::switch_target(Player * player) {
    TargetWidget * targetWidget = findChild<TargetWidget *>();
    if(player) {
        targetWidget->select_target(*player, true);
    }
    else {
        targetWidget->unselect_target();
    }
}
