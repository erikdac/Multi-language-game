#include "player.h"

#include <cmath>

#define MOVEMENT_STEP   5
#define TURN_SPEED      0.1

Player::Player(unsigned int x, unsigned int y, unsigned int z, double direction)
    : _x(x)
    , _y(y)
    , _z(z)
    , _direction(direction)
{

}

Player::~Player() {

}

void Player::moveForward() {
    _x += MOVEMENT_STEP*sin(_direction);
    _y += MOVEMENT_STEP*cos(_direction);
}

void Player::moveBackward() {
    _x -= MOVEMENT_STEP*sin(_direction);
    _y -= MOVEMENT_STEP*cos(_direction);
}

void Player::turnLeft() {
    _direction += TURN_SPEED;
    if(_direction > M_PI) {
        _direction -= 2*M_PI;
    }
}

void Player::turnRight() {
    _direction -= TURN_SPEED;
    if(_direction < M_PI) {
        _direction += 2*M_PI;
    }
}
