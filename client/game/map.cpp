#include "map.h"

Player * _player;

void initializeMap() {
    // TODO: Get player from server
    _player = new Player(100, 100);
}
