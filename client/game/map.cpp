#include "map.h"
#include "json11/json11.hpp"

#include <iostream>
#include <vector>

Player * _player;

std::vector<Player> _others;

using namespace json11;

void cleanMap() {
    delete _player;
    _others.clear();
}

void parse_map(const Json data) {
    Json::array players = data["Players"].array_items();
    for(unsigned int i = 0; i < players.size(); ++i) {
        _others.push_back(parse_player(players[i]));
    }
}

Player parse_player(const Json player) {
    std::string name = player["Name"].string_value();
    int x = player["X"].number_value();
    int y = player["Y"].number_value();
    int level = player["Level"].number_value();
    int health = player["Health"].number_value();
    return Player(name, x, y, level, health);
}
