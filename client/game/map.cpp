#include "map.h"
#include "json11/json11.hpp"
#include "ui/onlinewidget.h"
#include "ui/loginwidget.h"
#include "game/objects/water.h"
#include "game/objects/grass.h"
#include "game/objects/stone.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <QWidget>
#include <QLabel>

Self * _self;

std::vector<Actor *> _actors;

std::vector<Environment *> _environment;

using namespace json11;

void clearActors() {
    for(const Actor * a : _actors) {
        delete a;
    }
    _actors.clear();
}

void clearEnvironment() {
    for(const Environment * e : _environment) {
        delete e;
    }
    _environment.clear();
}

void map::cleanMap() {
    delete _self;
    clearActors();
    clearEnvironment();
}

Player * map::parse_player(const Json player) {
    std::string name = player["Name"].string_value();
    int x = player["X"].number_value();
    int y = player["Y"].number_value();
    int level = player["Level"].number_value();
    int health = player["Health"].number_value();
    int mana = player["Mana"].number_value();
    return new Player(name, x, y, level, health, mana);
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
    _actors.clear();
    for (const Json & p : players) {
        _actors.push_back(map::parse_player(p));
    }

    const Json::array environments = data["Environment"].array_items();
    clearEnvironment();
    for (const Json & e : environments) {
        _environment.push_back(parse_environment(e));
    }
}

// TODO: Old player never deleted!
void map::update_player(const Json data, TargetWidget * targetWidget) {
    Actor * actor = map::parse_player(data["Player"]);

    auto it = std::find_if(_actors.begin(), _actors.end(), [&actor](const Actor * a) {return a->name() == actor->name();});
    if(it != _actors.end()) {
        if (actor->name() == targetWidget->target()) {
            targetWidget->update_target(actor);
        }
        *it = actor;
    }
    else {
        _actors.push_back(actor);
    }
}

void map::remove_player(const Json data, TargetWidget * targetWidget) {
    Actor * actor = map::parse_player(data["Player"]);

    auto it = std::find_if(_actors.begin(), _actors.end(), [&actor](const Actor * a) {return a->name() == actor->name();});
    if (it != _actors.end()) {
        if (actor->name() == targetWidget->target()) {
            targetWidget->unselect_target();
        }
        *it = _actors[_actors.size()-1];
        delete _actors[_actors.size()-1];
        _actors.erase(_actors.begin() + _actors.size()-1);
    }
}

Actor * map::actor_at_position(const int x, const int y) {
    for (Actor * a : _actors) {
        if (a->x() == x && a->y() == y) {
            return a;
        }
    }
    return 0;
}

bool map::walkable(const int x, const int y) {
    for (Environment * e : _environment) {
        if (e->x() == x && e->y() == y && e->isWalkable() == false) {
            return false;
        }
    }
    return true;
}
