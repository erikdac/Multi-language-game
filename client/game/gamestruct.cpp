#include "gamestruct.h"
#include "json/json11.hpp"
#include "ui/gamewidget.h"
#include "ui/loginwidget.h"
#include "objects/water.h"
#include "objects/grass.h"
#include "objects/stone.h"
#include "objects/troll.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <QWidget>
#include <cassert>

// TODO: Maybe make this namespace as a class instead?

// TODO: Move out the parsing parts to a separate source file.

using namespace json11;

Self * _self;
std::vector<Actor *> _actors;
std::vector<Environment *> _environment;

void gamestruct::set_self(const Player * player) {
    _self = new Self(std::move(player));
}

Self * gamestruct::self() {
    return _self;
}

std::vector<Actor *> gamestruct::actors() {
    return _actors;
}

std::vector<Environment *> gamestruct::environment() {
    return _environment;
}

void clearActors() {
    for (auto a : _actors) {
        delete a;
    }
    _actors.clear();
}

void clearEnvironment() {
    for (auto e : _environment) {
        delete e;
    }
    _environment.clear();
}

void gamestruct::clear() {
    delete _self;
    clearActors();
    clearEnvironment();

    assert(_actors.size() == 0);
    assert(_environment.size() == 0);
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
    } else {
        std::string error = "No environment type specified in JSON!";
        std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
        std::cerr << "\tError: " << error << std::endl;
        return 0;
    }
}

// TODO: Should check that the values exists.
Player * parse_player(const Json player) {
    std::string name = player["Name"].string_value();
    int x = player["X"].number_value();
    int y = player["Y"].number_value();
    int level = player["Level"].number_value();
    int health = player["Health"].number_value();
    int mana = player["Mana"].number_value();
    return new Player(name, x, y, level, health, mana);
}

// TODO: Should check that the values exists.
// TODO: Should be parse_creature() in the future.
Troll * parse_troll(const Json creature) {
    std::string name = creature["Name"].string_value();
    int x = creature["X"].number_value();
    int y = creature["Y"].number_value();
    int health = creature["Health"].number_value();
    int mana = creature["Mana"].number_value();
    return new Troll(name, x, y, health, mana);
}

void gamestruct::parse_map(const Json data, TargetWidget * targetWidget) {
    clearEnvironment();
    const Json::array environments = data["Environment"].array_items();
    for (const Json & e : environments) {
        _environment.push_back(parse_environment(e));
    }

    const std::string target = targetWidget->target();
    bool targetFound = false;
    clearActors();

    const Json::array players = data["Players"].array_items();
    for (const Json & p : players) {
        _actors.push_back(parse_player(p));
        if (_actors.back()->name() == target) {
            targetWidget->update_target(_actors.back());
            targetFound = true;
        }
    }
    assert(targetWidget->target() == target || !targetFound);

    const Json::array creatures = data["Creatures"].array_items();
    for (const Json & c : creatures) {
        _actors.push_back(parse_troll(c));
        if (_actors.back()->name() == target) {
            targetWidget->update_target(_actors.back());
            targetFound = true;
        }
    }
    assert(targetWidget->target() == target || !targetFound);

    if (!targetFound) {
        targetWidget->unselect_target();
        assert(targetWidget->target() == "");
    }
}

void update_actor(Actor * actor, std::vector<Actor *> & vec, TargetWidget * targetWidget) {
    if (actor != 0) {
        auto it = std::find_if(vec.begin(), vec.end(), [&actor](const Actor * a) {return a->name() == actor->name();});
        if (it != vec.end()) {
            if (actor->name() == targetWidget->target()) {
                targetWidget->update_target(actor);
                assert(targetWidget->target() != "");
            }
            delete *it;
            *it = actor;
        } else {
            vec.push_back(actor);
        }
    } else {
        std::string error = "NULL Actor recieved!";
        std::cerr << "Line: " << __LINE__ << " FILE: " << __FILE__ << std::endl;
        std::cerr << "\tError: " << error << std::endl;
    }
}

void gamestruct::update_player(const Json data, TargetWidget * targetWidget) {
    Actor * actor = parse_player(data["Player"]);
    update_actor(actor, _actors, targetWidget);
}

void gamestruct::update_creature(const Json data, TargetWidget * targetWidget) {
    Actor * actor = parse_troll(data["Creature"]);
    update_actor(actor, _actors, targetWidget);
}

void gamestruct::remove_actor(const Json data, TargetWidget * targetWidget) {
    const std::string name = data["Name"].string_value();

    auto it = std::find_if(_actors.begin(), _actors.end(), [&name](const Actor * a) {return a->name() == name;});
    if (it != _actors.end()) {
        if (name == targetWidget->target()) {
            targetWidget->unselect_target();
        }
        delete *it;
        *it = _actors.back();
        _actors.pop_back();
    }
}

Actor * gamestruct::actor_at_position(const double x, const double y) {
    for (Actor * a : _actors) {
        if (a->x() <= x && a->x() + 1 > x && a->y() <= y && a->y() + 1 > y) {
            return a;
        }
    }
    return 0;
}

bool gamestruct::walkable(const int x, const int y) {
    for (const Environment * e : _environment) {
        if (e->x() == x && e->y() == y) {
            return e->isWalkable();
        }
    }
    return true;
}
