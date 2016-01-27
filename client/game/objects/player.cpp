#include "player.h"
#include "json11/json11.hpp"
#include "network/connection.h"
#include "game/map.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Player::Player(std::string name, unsigned int x, unsigned int y, unsigned int level, unsigned int health)
    : _name(name), _x(x), _y(y), _level(level), _health(health) {

}

Player::~Player() {

}

std::string Player::name() const {
    return _name;
}

unsigned int Player::x() const {
    return _x;
}

unsigned int Player::y() const {
    return _y;
}

unsigned int Player::level() const {
    return _level;
}

unsigned int Player::health() const {
    return _health;
}

unsigned int Player::max_health() const {
    return 100 + (_level - 1)*10;
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
    float x = (-0.5f + _x - _player->x())/VIEW_WIDTH;
    float y = -(0.5f + _y - _player->y())/VIEW_HEIGHT;

    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();
}
