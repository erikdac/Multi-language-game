#include "polygon.h"

Vertex::Vertex(float x, float y) : _x(x), _y(y) {

}

float Vertex::x() const {
	return _x;
}
float Vertex::y() const {
	return _y;
}

Color::Color(int r, int g, int b) : _r(r), _g(g), _b(b) {

}

int Color::r() const {
	return _r;
}

int Color::g() const {
	return _g;
}

int Color::b() const {
	return _b;
}

Polygon::Polygon(Color color, std::vector<Vertex> verticies) 
	: _color(color)
	, _verticies(verticies) {

}

Color Polygon::color() const {
	return _color;
}

std::vector<Vertex> Polygon::verticies() const {
	return _verticies;
}
