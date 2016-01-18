#include "player.h"
#include "../map.h"

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

void Player::load_graphics() {
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, -0.5f/VIEW_HEIGHT);
        glVertex2f(0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
        glVertex2f(-0.5f/VIEW_WIDTH, 0.5f/VIEW_HEIGHT);
    glEnd();
}
