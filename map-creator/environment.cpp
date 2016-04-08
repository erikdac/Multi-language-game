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

std::string Environment::to_string() const {
	std::string str = "";
	switch(_type) {
		case Environment::GRASS:
			str = "grass";
			break;
		case Environment::WATER:
			str = "water";
			break;
	}
	return str + " " + std::to_string(_x) + " " + std::to_string(_y) + "\n";
}
