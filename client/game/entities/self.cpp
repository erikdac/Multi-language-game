#include "self.h"
#include "external/json11/json11.hpp"
#include "network/connection.h"
#include "game/gamestruct.h"

#include <iostream>
#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Self::Self(const Player & player) : Player(player) {

}

Self::~Self() {

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
    connection::output(data);
}

// TODO: Should use a timer instead of fixed intervall.
void Self::update() {
    if (std::abs(_x - _visualX) < 0.06) {
        _visualX = _x;
    } else if (_x < _visualX) {
        _visualX -= 0.04;
    } else if (_x > _visualX) {
        _visualX += 0.04;
    }

    if (std::abs(_y - _visualY) < 0.06) {
        _visualY = _y;
    } else if (_y < _visualY) {
        _visualY -= 0.04;
    } else if (_y > _visualY) {
        _visualY += 0.04;
    }
}

void Self::draw() const {
    float x = -0.5f/VIEW_WIDTH;
    float y = -0.5f/VIEW_HEIGHT;

    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();
}
