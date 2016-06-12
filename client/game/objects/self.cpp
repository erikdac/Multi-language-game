#include "self.h"
#include "json11/json11.hpp"
#include "network/connection.h"
#include "game/map.h"

#include <iostream>
#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Self::Self(const Player & player) : Player(player) {

}

Self::~Self() {

}

void Self::moveUp() {
    --_y;
    sendMovement();
}

void Self::moveDown() {
    ++_y;
    sendMovement();
}

void Self::moveLeft() {
    --_x;
    sendMovement();
}

void Self::moveRight() {
    ++_x;
    sendMovement();
}

void Self::update_health(const int health) {
    _health = health;
}

void Self::update_position(const int x, const int y) {
    std::cout << "LOL" << std::endl;
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
