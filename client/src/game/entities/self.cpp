#include "self.h"
#include "json11/json11.hpp"
#include "network/connection.h"
#include "game/gamestruct.h"

#include <QPainter>
#include <QOpenGLTexture>

Self::Self() {

}

Self::Self(const Player & player) : Player(player) {

}

Self::~Self() {

}

float Self::visualX() const {
    return _visualX;
}

float Self::visualY() const {
    return _visualY;
}


void Self::moveUp() {
    if (gamestruct::walkable(_x, _y - 1)) {
        set_position(_x, _y - 1);
        sendMovement();
    }
}

void Self::moveDown() {
    if (gamestruct::walkable(_x, _y + 1)) {
        set_position(_x, _y + 1);
        sendMovement();
    }
}

void Self::moveLeft() {
    if (gamestruct::walkable(_x - 1, _y)) {
        set_position(_x - 1, _y);
        sendMovement();
    }
}

void Self::moveRight() {
    if (gamestruct::walkable(_x + 1, _y)) {
        set_position(_x + 1, _y);
        sendMovement();
    }
}

void Self::moveUpRight() {
    if (gamestruct::walkable(_x + 1, _y - 1)) {
        set_position(_x + 1, _y - 1);
        sendMovement();
    }
}

void Self::moveUpLeft() {
    if (gamestruct::walkable(_x - 1, _y - 1)) {
        set_position(_x - 1, _y - 1);
        sendMovement();
    }
}

void Self::moveDownRight() {
    if (gamestruct::walkable(_x + 1, _y + 1)) {
        set_position(_x + 1, _y + 1);
        sendMovement();
    }
}

void Self::moveDownLeft() {
    if (gamestruct::walkable(_x - 1, _y + 1)) {
        set_position(_x - 1, _y + 1);
        sendMovement();
    }
}

void Self::set_health(const int health) {
    _health = health;
}

void Self::set_position(const int x, const int y) {
    _x = x;
    _y = y;
}

void Self::fix_position(const int x, const int y) {
    set_position(x, y);
    _visualX = _x;
    _visualY = _y;
}

void Self::sendMovement() const {
    const json11::Json data = json11::Json::object {
        {"Type", "Movement"},
        {"ToX", std::to_string(_x)},
        {"ToY", std::to_string(_y)}
    };
    connection::write(data);
}
