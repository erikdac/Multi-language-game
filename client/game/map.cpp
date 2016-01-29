#include "map.h"
#include "json11/json11.hpp"
#include "ui/onlinewidget.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <QWidget>
#include <QLabel>

Player * _self;

std::vector<Player> _other_players;
std::mutex others_mutex;

TargetWidget * _target_widget;       // TODO: Find better place for this.

using namespace json11;

void map::cleanMap() {
    delete _self;
    _other_players.clear();
}

void check_target() {
    std::string name = _target_widget->findChild<QLabel *>("Name")->text().toStdString();
    bool found = false;
    others_mutex.lock();
    for(unsigned int i = 0; i < _other_players.size() && !found; ++i) {
        if(_other_players[i].name() == name) {
            found = true;
        }
    }
    others_mutex.unlock();
    if(!found) {
        _target_widget->unselect_target();
    }
}

void map::parse_map(const Json data) {
    Json::array players = data["Players"].array_items();
    others_mutex.lock();
    _other_players.clear();
    for(unsigned int i = 0; i < players.size(); ++i) {
        _other_players.push_back(map::parse_player(players[i]));
    }
    others_mutex.unlock();
    check_target();
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
}

void map::remove_player(const Json data) {
    Player player = map::parse_player(data["Player"]);

    others_mutex.lock();
    auto it = std::find (_other_players.begin(), _other_players.end(), player);
    if(it != _other_players.end()) {
        *it = _other_players[_other_players.size()-1];
        _other_players.erase(_other_players.begin() + _other_players.size()-1);
    }
    others_mutex.unlock();
    check_target();
}

Player map::parse_player(const Json player) {
    std::string name = player["Name"].string_value();
    int x = player["X"].number_value();
    int y = player["Y"].number_value();
    int level = player["Level"].number_value();
    int health = player["Health"].number_value();
    return Player(name, x, y, level, health);
}

Player * map::player_at_position(unsigned int x, unsigned int y) {
    for(unsigned int i = 0; i < _other_players.size(); ++i) {
        if(_other_players[i].x() == x && _other_players[i].y() == y) {
            return &_other_players[i];
        }
    }
    return 0;
}
