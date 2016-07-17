#include "self.h"
#include "json11/json11.hpp"
#include "network/connection.h"
#include "game/map.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Self::Self(const Player * player) : Player(*player) {

}

Self::~Self() {

}

void Self::moveUp() {
    if (map::walkable(_x, _y - 1)) {
        --_y;
        sendMovement();
    }
}

void Self::moveDown() {
    if (map::walkable(_x, _y + 1)) {
        ++_y;
        sendMovement();
    }
}

void Self::moveLeft() {
    if (map::walkable(_x - 1, _y)) {
        --_x;
        sendMovement();
    }
}

void Self::moveRight() {
    if (map::walkable(_x + 1, _y)) {
        ++_x;
        sendMovement();
    }
}

void Self::moveUpRight() {
    if (map::walkable(_x + 1, _y - 1)) {
        ++_x;
        --_y;
        sendMovement();
    }
}

void Self::moveUpLeft() {
    if (map::walkable(_x - 1, _y - 1)) {
        --_x;
        --_y;
        sendMovement();
    }
}

void Self::moveDownRight() {
    if (map::walkable(_x + 1, _y + 1)) {
        ++_x;
        ++_y;
        sendMovement();
    }
}

void Self::moveDownLeft() {
    if (map::walkable(_x - 1, _y + 1)) {
        --_x;
        ++_y;
        sendMovement();
    }
}

void Self::update_health(const int health) {
    _health = health;
}

void Self::update_position(const int x, const int y) {
    _x = x;
    _y = y;
}

void Self::sendMovement() const {
    const json11::Json data = json11::Json::object {
        {"Type", "Movement"},
        {"ToX", std::to_string(_x)},
        {"ToY", std::to_string(_y)}
    };
    connection::output(data);
}

void Self::load_graphics() const {
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
