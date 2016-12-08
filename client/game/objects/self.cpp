#include "self.h"
#include "json/json11.hpp"
#include "network/connection.h"
#include "game/gamestruct.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Self::Self(const Player * player) : Player(*player) {

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
        ++_x;
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
