#include "map.h"
#include "json11/json11.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>

Player * _player;

std::vector<Player> _others;
static std::mutex others_mutex;

using namespace json11;

void map::cleanMap() {
    delete _player;
    _others.clear();
}

void map::parse_map(const Json data) {
    Json::array players = data["Players"].array_items();
    others_mutex.lock();
    for(unsigned int i = 0; i < players.size(); ++i) {
        _others.push_back(map::parse_player(players[i]));
    }
    others_mutex.unlock();
}

void map::update_player(const Json data) {
    Player player = map::parse_player(data["Player"]);

    others_mutex.lock();
    auto it = std::find (_others.begin(), _others.end(), player);
    if(it != _others.end()) {
        *it = player;
    }
    else {
        _others.push_back(player);
    }
    others_mutex.unlock();
}

void map::remove_player(const Json data) {
    Player player = map::parse_player(data["Player"]);

    others_mutex.lock();
    auto it = std::find (_others.begin(), _others.end(), player);
    if(it != _others.end()) {
        *it = _others[_others.size()-1];
        _others.erase(_others.begin() + _others.size()-1);
    }
    others_mutex.unlock();
}

Player map::parse_player(const Json player) {
    std::string name = player["Name"].string_value();
    int x = player["X"].number_value();
    int y = player["Y"].number_value();
    int level = player["Level"].number_value();
    int health = player["Health"].number_value();
    return Player(name, x, y, level, health);
}
