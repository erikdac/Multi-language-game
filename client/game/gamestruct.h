#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H

#include "objects/actor.h"
#include "objects/player.h"
#include "json/json11.hpp"
#include "ui/targetwidget.h"
#include "objects/environment.h"
#include "game/objects/self.h"

#include <vector>

Player * parse_player(const json11::Json);

namespace gamestruct {
    void set_self(const Player *);
    Self * self();
    std::vector<const Actor *> actors();
    std::vector<const Environment *> environment();
    void clear();
    void parse_map(const json11::Json, TargetWidget *);
    void update_player(const json11::Json data, TargetWidget *);
    void update_creature(const json11::Json data, TargetWidget *);
    void remove_actor(const json11::Json data, TargetWidget *);
    Actor * actor_at_position(const double x, const double y);
    bool walkable(const int x, const int y);
}

#endif
