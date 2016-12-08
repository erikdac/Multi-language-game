#include "troll.h"
#include "game/gamestruct.h"

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
    std::vector<float> pos = relativePos(
                _x, _y, gamestruct::self()->x(), gamestruct::self()->y()
            );

    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1] + (1.0f/VIEW_HEIGHT));
        glVertex2f(pos[0], pos[1] + (1.0f/VIEW_HEIGHT));
    glEnd();
}
