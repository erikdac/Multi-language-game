#include "player.h"

#include <cmath>

#define MOVEMENT_STEP   1

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
}

void Player::moveDown() {
    ++_y;
}

void Player::moveLeft() {
    --_x;
}

void Player::moveRight() {
    ++_x;
}
