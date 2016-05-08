#include "environment.h"
#include "game/map.h"

#include <QPainter>
#include <QOpenGLTexture>
#include <GL/glut.h>

Environment::Environment(const int x, const int y) : _x(x), _y(y) {

}

int Environment::x() const {
    return _x;
}

int Environment::y() const {
    return _y;
}

void Environment::load_graphics() const {
    float x = (-0.5f + _x - _self->x())/VIEW_WIDTH;
    float y = -(0.5f + _y - _self->y())/VIEW_HEIGHT;

    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(x, y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y);
        glVertex2f(x + (1.0f/VIEW_WIDTH), y + (1.0f/VIEW_HEIGHT));
        glVertex2f(x, y + (1.0f/VIEW_HEIGHT));
    glEnd();
}
