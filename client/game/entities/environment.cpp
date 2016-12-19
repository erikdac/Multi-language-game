#include "environment.h"

Environment::Environment(const int x, const int y) : _x(x), _y(y) {

}

Environment::~Environment() {

}

int Environment::x() const {
    return _x;
}

int Environment::y() const {
    return _y;
}
