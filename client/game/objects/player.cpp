#include "player.h"
#include "json11/json11.hpp"
#include "network/connection.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Player::Player(unsigned int x, unsigned int y) : _x(x), _y(y) {

}

Player::~Player() {

}

unsigned int Player::x() const {
    return _x;
}

unsigned int Player::y() const {
    return _y;
}

void Player::moveUp() {
    --_y;
    sendMovement();
}

void Player::moveDown() {
    ++_y;
    sendMovement();
}

void Player::moveLeft() {
    --_x;
    sendMovement();
}

void Player::moveRight() {
    ++_x;
    sendMovement();
}

void Player::sendMovement() {
    const json11::Json data = json11::Json::object {
        {"Type", "Movement"},
        {"ToX", std::to_string(_x)},
        {"ToY", std::to_string(_y)}
    };
    connection::output(data);
}

void Player::load_graphics() {
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
        glVertex2f(-0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
    glEnd();
}
