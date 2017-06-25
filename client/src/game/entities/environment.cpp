#include "environment.h"
#include "game/gamestruct.h"

#include <QOpenGLTexture>

Environment::Environment() {

}

Environment::Environment(int x, int y, bool walkable, std::vector<graphics::Polygon> * polygons)
	: _x(x)
	, _y(y)
	, _walkable(walkable)
	, _polygons(polygons) {

}

Environment::~Environment() {

}

int Environment::x() const {
    return _x;
}

int Environment::y() const {
    return _y;
}

bool Environment::walkable() const {
	return _walkable;
}

void Environment::update(float deltaTime) {

}

void Environment::draw() const {
    std::vector<float> pos = relativePos(
        _x, _y,
        gamestruct::self()->visualX(), gamestruct::self()->visualY()
    );
    glBegin(GL_QUADS);
    for (const graphics::Polygon & polygon : *_polygons) {
        const graphics::Color & c = polygon.color();
        glColor3f((float)c.r() / graphics::RGB_MAX, (float)c.g() / graphics::RGB_MAX, (float)c.b() / graphics::RGB_MAX);
        for (const graphics::Vertex & vertex : polygon.verticies()) {
            glVertex3f(
                pos[0] + (vertex.x() / graphics::VIEW_WIDTH),
                pos[1] + (vertex.y() / graphics::VIEW_HEIGHT),
                vertex.z()
    		);
	    }
	}
    glEnd();
}
