#include <iostream>

#include "creature.hpp"

static int id = 0;

Creature::Creature(const Creature::Type type, const int x, const int y) : _x(x), _y(y), _type(type) {

}

std::string Creature::to_string() const {
	std::string str = "";
	switch(_type) {
	case Creature::TROLL:
		str = "TROLL";
		break;
	}
	std::string name = str + std::to_string(id++);
	return str + " " + name + " " + std::to_string(_x) + " " + std::to_string(_y);
}
