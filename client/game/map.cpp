#include "map.h"
#include "json11/json11.hpp"
#include "ui/onlinewidget.h"
#include "ui/loginwidget.h"
#include "game/objects/water.h"
#include "game/objects/grass.h"
#include "game/objects/stone.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <QWidget>
#include <QLabel>

Self * _self;

std::vector<Player> _other_players;
std::mutex others_mutex;

std::vector<Environment *> _environment;
std::mutex environment_mutex;

using namespace json11;

void clearEnvironment() {
    for(const Environment * e : _environment) {
        delete e;
    }
    _environment.clear();
}

void map::cleanMap() {
    delete _self;
    _other_players.clear();
    clearEnvironment();
}

void check_target() {
    TargetWidget * targetWidget = OnlineWidget::instance()->target_widget();
    std::string target_name = targetWidget->target();
    bool found = false;
    others_mutex.lock();
    for(Player & p : _other_players) {
        if (p.name() == target_name) {
            found = true;
            targetWidget->update_target(p);
            break;
        }
    }
    others_mutex.unlock();
    if(!found) {
        targetWidget->unselect_target();
    }
}

Player map::parse_player(const Json player) {
    std::string name = player["Name"].string_value();
    int x = player["X"].number_value();
    int y = player["Y"].number_value();
    int level = player["Level"].number_value();
    int health = player["Health"].number_value();
    int mana = player["Mana"].number_value();
    return Player(name, x, y, level, health, mana);
}

Environment * parse_environment(const Json & environment) {
    int x = environment["X"].number_value();
    int y = environment["Y"].number_value();

    const std::string type = environment["Type"].string_value();
    if(type == "GRASS") {
        return new Grass(x, y);
    }
    else if(type == "STONE") {
        return new Stone(x, y);
    }
    else if(type == "WATER") {
        return new Water(x, y);
    }
    return 0;
}

void map::parse_map(const Json data) {
    const Json::array players = data["Players"].array_items();
    others_mutex.lock();
    _other_players.clear();
    for (const Json & p : players) {
        _other_players.push_back(map::parse_player(p));
    }
    others_mutex.unlock();
    check_target();

    const Json::array environments = data["Environment"].array_items();
    environment_mutex.lock();
    clearEnvironment();
    for (const Json & e : environments) {
        _environment.push_back(parse_environment(e));
    }
    environment_mutex.unlock();
}

void map::update_player(const Json data) {
    Player player = map::parse_player(data["Player"]);

    others_mutex.lock();
    auto it = std::find (_other_players.begin(), _other_players.end(), player);
    if(it != _other_players.end()) {
        *it = player;
    }
    else {
        _other_players.push_back(player);
    }
    others_mutex.unlock();

    TargetWidget * targetWidget = OnlineWidget::instance()->target_widget();
    std::string target_name = targetWidget->target();
    if(player.name() == target_name) {
        targetWidget->update_target(player);
    }
}

void map::remove_player(const Json data) {
    Player player = map::parse_player(data["Player"]);

    others_mutex.lock();
    auto it = std::find (_other_players.begin(), _other_players.end(), player);
    if (it != _other_players.end()) {
        *it = _other_players[_other_players.size()-1];
        _other_players.erase(_other_players.begin() + _other_players.size()-1);
    }
    others_mutex.unlock();

    TargetWidget * targetWidget = OnlineWidget::instance()->target_widget();
    std::string target_name = targetWidget->target();
    if (player.name() == target_name) {
        targetWidget->unselect_target();
    }
}

Player * map::player_at_position(const unsigned int x, const unsigned int y) {
    for (Player & p : _other_players) {
        if( p.x() == x && p.y() == y) {
            return &p;
        }
    }
    return 0;
}

bool map::walkable(const int x, const int y) {
    bool isWalkable = true;
    environment_mutex.lock();
    for (Environment * e : _environment) {
        if (e->x() == x && e->y() == y && e->isWalkable() == false) {
            isWalkable = false;
            break;
        }
    }
    environment_mutex.unlock();
    return isWalkable;
}
