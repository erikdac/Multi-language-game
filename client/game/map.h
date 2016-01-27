#ifndef MAP_H
#define MAP_H

#include "objects/player.h"
#include "json11/json11.hpp"

#include <vector>

extern Player * _player;
extern std::vector<Player> _others;

void cleanMap();
void parse_map(const json11::Json);
Player parse_player(const json11::Json);

#endif // MAP_H
