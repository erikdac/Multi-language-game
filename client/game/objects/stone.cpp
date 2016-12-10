#include "stone.h"
#include "game/gamestruct.h"


#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>
#include <vector>

Stone::~Stone() {

}

bool Stone::isWalkable() const {
    return false;
}

void Stone::update() {

}

void Stone::draw() const {
    std::vector<float> pos = relativePos(
                _x, _y,
                gamestruct::self()->visualX(), gamestruct::self()->visualY()
            );

    glBegin(GL_QUADS);
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1] + (1.0f/VIEW_HEIGHT));
        glVertex2f(pos[0], pos[1] + (1.0f/VIEW_HEIGHT));
    glEnd();
}
