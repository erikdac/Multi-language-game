#include "player.h"
#include "game/gamestruct.h"


#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Player::Player(std::string name, int x, int y, int level, int health, int mana) {
    _name = name;
    _x = x;
    _y = y;
    _direction = 0;
    _level = level;
    _health = health;
    _mana = mana;

}

Player::~Player() {

}

int Player::max_health() const {
    return 100 + (_level - 1)*5;
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
    std::vector<float> pos = relativePos(
                _x, _y, gamestruct::self()->x(), gamestruct::self()->y()
            );

    glBegin(GL_QUADS);
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1] + (1.0f/VIEW_HEIGHT));
        glVertex2f(pos[0], pos[1] + (1.0f/VIEW_HEIGHT));
    glEnd();
}
