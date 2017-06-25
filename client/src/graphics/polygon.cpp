#include "polygon.h"

using namespace graphics;

graphics::Color::Color(int r, int g, int b) : _r(r), _g(g), _b(b) {

}

int graphics::Color::r() const {
    return _r;
}

int graphics::Color::g() const {
    return _g;
}

int graphics::Color::b() const {
    return _b;
}

graphics::Vertex::Vertex(float x, float y, float z) : _x(x), _y(y), _z(z) {

}

float graphics::Vertex::x() const {
    return _x;
}

float graphics::Vertex::y() const {
    return _y;
}

float graphics::Vertex::z() const {
    return _z;
}

graphics::Polygon::Polygon(Color color, std::vector<Vertex> verticies)
	: _color(color)
	, _verticies(verticies) {

}

Color graphics::Polygon::color() const {
	return _color;
}

std::vector<Vertex> graphics::Polygon::verticies() const {
	return _verticies;
}
