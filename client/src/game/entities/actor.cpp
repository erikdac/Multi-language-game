#include "actor.h"

std::string Actor::name() const {
    return _name;
}

int Actor::x() const {
    return _x;
}

int Actor::y() const {
    return _y;
}

int Actor::level() const {
    return _level;
}

int Actor::health() const {
    return _health;
}

int Actor::mana() const {
    return _mana;
}
