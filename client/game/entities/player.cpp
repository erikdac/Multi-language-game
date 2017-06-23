#include "player.h"
#include "game/gamestruct.h"
#include "config.h"

#include <QPainter>
#include <QOpenGLTexture>

Player::Player() {

}

Player::Player(std::string name, int x, int y, int level, int health, int mana) {
    _name = name;
    _x = x;
    _y = y;
    _visualX = _x;
    _visualY = _y;
    _level = level;
    _health = health;
    _mana = mana;
}

Player::~Player() {

}

int Player::max_health() const {
    return 100 + (_level - 1) * 5;
}

int Player::max_mana() const {
    return 20 + (_level - 1) * 2;
}

unsigned int Player::distance_to_player(const Player & p) const {
    int x = std::abs(_x - p.x());
    int y = std::abs(_y - p.y());
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

void Player::update(float deltaTime) {
    float dist = deltaTime / MOVEMENT_DELAY;
    if (std::abs(_x - _visualX) < dist * 1.5) {
        _visualX = _x;
    } else if (_x < _visualX) {
        _visualX -= dist;
    } else if (_x > _visualX) {
        _visualX += dist;
    }

    if (std::abs(_y - _visualY) < dist * 1.5) {
        _visualY = _y;
    } else if (_y < _visualY) {
        _visualY -= dist;
    } else if (_y > _visualY) {
        _visualY += dist;
    }
}

void Player::draw() const {
    std::vector<float> pos = relativePos(
                _visualX, _visualY,
                gamestruct::self()->visualX(), gamestruct::self()->visualY()
            );

    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(pos[0], pos[1], 0.0);
        glVertex3f(pos[0] + (1.0f / graphics::VIEW_WIDTH), pos[1], 0.0);
        glVertex3f(pos[0] + (1.0f / graphics::VIEW_WIDTH), pos[1] + (1.0f / graphics::VIEW_HEIGHT), 0.0);
        glVertex3f(pos[0], pos[1] + (1.0f / graphics::VIEW_HEIGHT), 0.0);
    glEnd();
}
