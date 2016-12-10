#include "grass.h"
#include "game/gamestruct.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Grass::~Grass() {

}

bool Grass::isWalkable() const {
    return true;
}

void Grass::update() {

}

void Grass::draw() const {
    std::vector<float> pos = relativePos(
                _x, _y,
                gamestruct::self()->visualX(), gamestruct::self()->visualY()
            );

    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1] + (1.0f/VIEW_HEIGHT));
        glVertex2f(pos[0], pos[1] + (1.0f/VIEW_HEIGHT));
    glEnd();
}
