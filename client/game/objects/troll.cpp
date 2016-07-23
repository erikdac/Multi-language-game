#include "troll.h"
#include "game/map.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Troll::Troll(std::string name, int x, int y, int health, int mana) {
    _name = name;
    _x = x;
    _y = y;
    _level = 1;
    _health = health;
    _mana = mana;
}

Troll::~Troll() {

}

int Troll::max_health() const {
    return 10;
}

int Troll::max_mana() const {
    return 10;
}

void Troll::load_graphics() const {
    float x = (-0.5f + _x - _self->x())/VIEW_WIDTH;
    float y = -(0.5f + _y - _self->y())/VIEW_HEIGHT;

    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();
}
