#ifndef MAP_H
#define MAP_H

#include "objects/player.h"
#include "json11/json11.hpp"
#include "ui/targetwidget.h"
#include "ui/playerwidget.h"
#include "objects/environment.h"
#include "game/objects/self.h"

#include <QWidget>
#include <vector>
#include <mutex>

extern Self * _self;
extern std::vector<Player> _other_players;
extern std::mutex others_mutex;
extern std::vector<Environment *> _environment;
extern std::mutex environment_mutex;

namespace map {
    void cleanMap();
    void parse_map(const json11::Json);
    void update_player(const json11::Json data, TargetWidget *);
    void remove_player(const json11::Json data, TargetWidget *);
    Player parse_player(const json11::Json);
    Player * player_at_position(const unsigned int x, const unsigned int y);
    bool walkable(const int x, const int y);
}
#endif // MAP_H
