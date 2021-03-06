#include "environment.hpp"

Environment::Environment(const Environment::Type type, const int x, const int y) : _x(x), _y(y), _type(type) {

}

std::string Environment::to_string() const {
	std::string str = "";
	char isWalkable;
	switch(_type) {
	case Environment::GRASS:
		str = "grass";
		isWalkable = 'T';
		break;
	case Environment::STONE:
		str = "stone";
		isWalkable = 'F';
		break;
	case Environment::WATER:
		str = "water";
		isWalkable = 'F';
		break;
	}
	return str + " " + std::to_string(_x) + " " + std::to_string(_y) + " " + isWalkable;
}
