#include "polygon_OLD2.h"

Color::Color() {

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

