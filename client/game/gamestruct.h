#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H

#include "entities/actor.h"
#include "entities/player.h"
#include "external/json11/json11.hpp"
#include "ui/targetwidget.h"
#include "entities/environment.h"
#include "game/entities/self.h"

#include <vector>

Player * parse_player(const json11::Json &);

namespace gamestruct {
    void set_self(const Player &);
    Self * self();
    std::vector<Actor *> actors();
    std::vector<Environment *> environment();
    void clear();
    void parse_map(const json11::Json &, TargetWidget *);
    void update_player(const json11::Json &, TargetWidget *);
    void update_creature(const json11::Json &, TargetWidget *);
    void remove_actor(const json11::Json &, TargetWidget *);
    Actor * actor_at_position(const double x, const double y);
    bool walkable(const int x, const int y);
}

#endif
