#include "water.h"
#include "game/gamestruct.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>


Water::~Water() {

}

bool Water::isWalkable() const {
    return false;
}

void Water::load_graphics() const {
    std::vector<float> pos = relativePos(
                _x, _y, gamestruct::self()->x(), gamestruct::self()->y()
            );

    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(pos[0], pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1]);
        glVertex2f(pos[0] + (1.0f/VIEW_WIDTH), pos[1] + (1.0f/VIEW_HEIGHT));
        glVertex2f(pos[0], pos[1] + (1.0f/VIEW_HEIGHT));
    glEnd();
}
