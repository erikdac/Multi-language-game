#ifndef MAP_H
#define MAP_H

#include "objects/player.h"
#include "json11/json11.hpp"
#include "ui/targetwidget.h"
#include "objects/environment.h"
#include "game/objects/self.h"

#include <vector>

extern Self * _self;
extern std::vector<Actor *> _actors;
extern std::vector<Environment *> _environment;

namespace map {
    void cleanMap();
    void parse_map(const json11::Json, TargetWidget *);
    void update_player(const json11::Json data, TargetWidget *);
    void update_troll(const json11::Json data, TargetWidget *);
    void remove_actor(const json11::Json data, TargetWidget *);
    Player * parse_player(const json11::Json);
    Actor * actor_at_position(const int x, const int y);
    bool walkable(const int x, const int y);
}
#endif // MAP_H
