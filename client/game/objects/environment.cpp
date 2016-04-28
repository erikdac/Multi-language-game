#include "environment.h"

Environment::Environment(const std::string type, const int x, const int y) : _x(x), _y(y) {
    if(type == "grass") {
        _type = Environment::GRASS;
    }
    else if(type == "water") {
        _type = Environment::WATER;
    }
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

void Environment::load_graphics() const {

}
