#include "movementcontroller.h"
#include "game/map.h"
#include "objects/self.h"

#include <chrono>
#include <cmath>

void MovementController::pushed(const char key) {
    _previousKey = _activeKey;
    _activeKey = key;
}

void MovementController::released(const char key) {
    if (key == _activeKey && _previousKey != 0) {
        _activeKey = _previousKey;
        _previousKey = 0;
    }
    else if (key == _activeKey) {
        _activeKey = 0;
    }
    else if (key == _previousKey) {
        _previousKey = 0;
    }
}

void MovementController::clear() {
    _activeKey = 0;
    _previousKey = 0;
}

auto last = std::chrono::high_resolution_clock::now();
bool expensiveLast = false;

bool MovementController::isReady() const {
    auto now = std::chrono::high_resolution_clock::now();
    int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if ((!expensiveLast && diff >= 250) || (expensiveLast && diff >= 250 * std::sqrt(2))) {
        last = now;
        return true;
    }
    return false;
}

void MovementController::execute() {
    if (!isReady()) {
        return;
    }

    if ((_activeKey == 'w' && _previousKey == 'a') || (_activeKey == 'a' && _previousKey == 'w')) {
        _self->moveUpLeft();
        expensiveLast = true;
    }
    else if ((_activeKey == 'w' && _previousKey == 'd') || (_activeKey == 'd' && _previousKey == 'w')) {
        _self->moveUpRight();
        expensiveLast = true;
    }
    else if ((_activeKey == 's' && _previousKey == 'a') || (_activeKey == 'a' && _previousKey == 's')) {
        _self->moveDownLeft();
        expensiveLast = true;
    }
    else if ((_activeKey == 's' && _previousKey == 'd') || (_activeKey == 'd' && _previousKey == 's')) {
        _self->moveDownRight();
        expensiveLast = true;
    }
    else if (_activeKey == 'w') {
        _self->moveUp();
        expensiveLast = false;
    }
    else if (_activeKey == 'a') {
        _self->moveLeft();
        expensiveLast = false;
    }
    else if (_activeKey == 's') {
        _self->moveDown();
        expensiveLast = false;
    }
    else if (_activeKey == 'd') {
        _self->moveRight();
        expensiveLast = false;
    }
}
