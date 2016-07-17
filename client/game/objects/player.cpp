#include "player.h"
#include "json11/json11.hpp"
#include "network/connection.h"
#include "game/map.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Player::Player() : _name("") {

}

Player::Player(std::string name, int x, int y, int level, int health, int mana)
    : _name(name), _x(x), _y(y), _level(level), _health(health), _mana(mana) {

}

Player::~Player() {

}

bool Player::operator==(const Player & other) const {
    return _name == other.name();
}

std::string Player::name() const {
    return _name;
}

int Player::x() const {
    return _x;
}

int Player::y() const {
    return _y;
}

int Player::level() const {
    return _level;
}

int Player::health() const {
    return _health;
}

int Player::max_health() const {
    return 100 + (_level - 1)*5;
}

int Player::mana() const {
    return _mana;
}

int Player::max_mana() const {
    return 20 + (_level - 1) * 2;
}

unsigned int Player::distance_to_player(const Player & p) const {
    int x = std::abs(_x - p.x());
    int y = std::abs(_y - p.y());
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

void Player::load_graphics() const {
    float x = (-0.5f + _x - _self->x())/VIEW_WIDTH;
    float y = -(0.5f + _y - _self->y())/VIEW_HEIGHT;

    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();
}
