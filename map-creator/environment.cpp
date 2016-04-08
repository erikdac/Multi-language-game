#include "environment.hpp"

Environment::Environment(const Environment::Type type, const int x, const int y) : _x(x), _y(y), _type(type) {

}

int Environment::x() const {
	return _x;
}

int Environment::y() const {
	return _y;
}

Environment::Type Environment::type() const {
	return _type;
}
