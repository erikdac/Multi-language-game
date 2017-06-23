#include "gamestruct.h"
#include "external/json11/json11.hpp"
#include "ui/gamewidget.h"
#include "ui/loginwidget.h"
#include "entities/troll.h"
#include "entities/player.h"
#include "entities/environment.h"
#include "parser.h"

#include <algorithm>
#include <vector>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <QWidget>
#include <cassert>
#include <QtDebug>

using namespace json11;

Self _self;
std::vector<Actor *> _actors;
std::vector<Environment> _environment;

void gamestruct::set_self(const Json & data) {
    Player * player = parser::parsePlayer(data["Player"]);
    _self = Self(*player);
    delete player;
}

Self * gamestruct::self() {
    return &_self;
}

std::vector<Actor *> gamestruct::actors() {
    return _actors;
}

std::vector<Environment> gamestruct::environment() {
    return _environment;
}

void clearActors() {
    for (auto a : _actors) {
        delete a;
    }
    _actors.clear();
}

void gamestruct::clear() {
    _environment.clear();
    clearActors();
    assert(_actors.empty());
}

void gamestruct::init_map(const Json & data) {
    _environment.clear();
    const Json::array environments = data["Environment"].array_items();
    for (const Json & e : environments) {
        _environment.push_back(parser::parseEnvironment(e));
    }

    clearActors();

    const Json::array players = data["Players"].array_items();
    for (const Json & p : players) {
        _actors.push_back(parser::parsePlayer(p));
    }

    const Json::array creatures = data["Creatures"].array_items();
    for (const Json & c : creatures) {
        _actors.push_back(parser::parseTroll(c));
    }
}

void gamestruct::new_map(const Json & data, TargetWidget * targetWidget) {
    _environment.clear();
    const Json::array environments = data["Environment"].array_items();
    for (const Json & e : environments) {
        _environment.push_back(parser::parseEnvironment(e));
    }

    const std::string target = targetWidget->target();
    bool targetFound = false;
    clearActors();

    const Json::array players = data["Players"].array_items();
    for (const Json & p : players) {
        _actors.push_back(parser::parsePlayer(p));
        if (_actors.back()->name() == target) {
            targetWidget->update_target(_actors.back());
            targetFound = true;
        }
    }
    assert(targetWidget->target() == target || !targetFound);

    const Json::array creatures = data["Creatures"].array_items();
    for (const Json & c : creatures) {
        _actors.push_back(parser::parseTroll(c));
        if (_actors.back()->name() == target) {
            targetWidget->update_target(_actors.back());
            targetFound = true;
        }
    }
    assert(targetWidget->target() == target || !targetFound);

    if (!targetFound && !targetWidget->target().empty()) {
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
        qCritical() << "NULL Actor as function parameter!";
    }
}

void gamestruct::update_player(const Json & data, TargetWidget * targetWidget) {
    Actor * actor = parser::parsePlayer(data["Player"]);
    update_actor(actor, _actors, targetWidget);
}

void gamestruct::update_creature(const Json & data, TargetWidget * targetWidget) {
    Actor * actor = parser::parseTroll(data["Creature"]);
    update_actor(actor, _actors, targetWidget);
}

void gamestruct::remove_actor(const Json & data, TargetWidget * targetWidget) {
    const std::string name = data["Name"].string_value();

    auto it = std::find_if(_actors.begin(), _actors.end(), [&name](const Actor * a) {return a->name() == name;});
    if (it != _actors.end()) {
        if (name == targetWidget->target()) {
            targetWidget->unselect_target();
        }
        if (_actors.size() > 1) {
            delete *it;
            *it = _actors.back();
            _actors.pop_back();
        } else {
            delete *it;
            _actors.erase(it);
        }
    }
}

void gamestruct::update_entities(float deltaTime) {
    for (Environment & e : _environment) {
        e.update(deltaTime);
    }

    for (auto a : _actors) {
        a->update(deltaTime);
    }

    _self.update(deltaTime);
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
    for (auto e : _environment) {
        if (e.x() == x && e.y() == y) {
            return e.walkable();
        }
    }
    return true;
}
