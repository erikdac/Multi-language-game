#include "polygon_OLD.h"

Vertex::Vertex() {

}

Vertex::Vertex(float x, float y, float z) : _x(x), _y(y), _z(z) {

}

float Vertex::x() const {
	return _x;
}

float Vertex::y() const {
	return _y;
}

float Vertex::z() const {
    return _z;
}
