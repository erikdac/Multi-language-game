#include "troll.h"
#include "game/gamestruct.h"

#include <QPainter>
#include <QOpenGLTexture>

Troll::Troll(std::string name, int x, int y, int health, int mana) {
    _name = name;
    _x = x;
    _y = y;
    _visualX = _x;
    _visualY = _y;
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

void Troll::update(float deltaTime) {

}

void Troll::draw() const {
    std::vector<float> pos = relativePos(
                _x, _y,
                gamestruct::self()->visualX(), gamestruct::self()->visualY()
            );

    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(pos[0], pos[1], 0.0);
        glVertex3f(pos[0] + (1.0f / graphics::VIEW_WIDTH), pos[1], 0.0);
        glVertex3f(pos[0] + (1.0f / graphics::VIEW_WIDTH), pos[1] + (1.0f / graphics::VIEW_HEIGHT), 0.0);
        glVertex3f(pos[0], pos[1] + (1.0f / graphics::VIEW_HEIGHT), 0.0);
    glEnd();
}
