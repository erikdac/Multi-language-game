#ifndef MAP_H
#define MAP_H

#include "objects/player.h"
#include "json11/json11.hpp"

#include <QWidget>
#include <vector>
#include <mutex>

extern Player * _player;
extern std::vector<Player> _other_players;
extern std::mutex others_mutex;

extern QWidget * _target_widget;

namespace map {
    void cleanMap();
    void parse_map(const json11::Json);
    void update_player(const json11::Json data);
    void remove_player(const json11::Json data);
    Player parse_player(const json11::Json);
    Player * player_at_position(unsigned int x, unsigned int y);
}
#endif // MAP_H
