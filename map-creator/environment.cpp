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

bool Environment::operator<(const Environment & other) const {
	if(_x == other.x()) {
		return _y < other.y();			
	}
	return _x < other.x();
}

bool Environment::operator!=(const Environment & other) const {
	return (_x != other.x() || _y != other.y());
}

std::string Environment::to_string() const {
	std::string str = "";
	switch(_type) {
		case Environment::GRASS:
			str = "GRASS";
			break;
		case Environment::WATER:
			str = "WATER";
			break;
	}
	return str + " " + std::to_string(_x) + " " + std::to_string(_y);
}
