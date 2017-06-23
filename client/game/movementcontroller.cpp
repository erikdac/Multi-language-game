#include "movementcontroller.h"
#include "config.h"

#include <chrono>
#include <cmath>
#include <vector>

bool newly_pushed = false;
std::vector<char> saved_releases;
bool expensiveLast = false;

void MovementController::execute(Self * const self) {
    if (!isReady() || _activeKey == 0) {
        return;
    }

    if ((_activeKey == 'w' && _previousKey == 'a') || (_activeKey == 'a' && _previousKey == 'w')) {
        self->moveUpLeft();
        expensiveLast = true;
    }
    else if ((_activeKey == 'w' && _previousKey == 'd') || (_activeKey == 'd' && _previousKey == 'w')) {
        self->moveUpRight();
        expensiveLast = true;
    }
    else if ((_activeKey == 's' && _previousKey == 'a') || (_activeKey == 'a' && _previousKey == 's')) {
        self->moveDownLeft();
        expensiveLast = true;
    }
    else if ((_activeKey == 's' && _previousKey == 'd') || (_activeKey == 'd' && _previousKey == 's')) {
        self->moveDownRight();
        expensiveLast = true;
    }
    else if (_activeKey == 'w') {
        self->moveUp();
        expensiveLast = false;
    }
    else if (_activeKey == 'a') {
        self->moveLeft();
        expensiveLast = false;
    }
    else if (_activeKey == 's') {
        self->moveDown();
        expensiveLast = false;
    }
    else if (_activeKey == 'd') {
        self->moveRight();
        expensiveLast = false;
    }

    if (newly_pushed) {
        newly_pushed = false;
        for (const char & c : saved_releases) {
            released(c);
        }
        saved_releases.clear();
    }
}

void MovementController::pushed(const char key) {
    _previousKey = _activeKey;
    _activeKey = key;
    newly_pushed = true;
}

void MovementController::released(const char key) {
    if (newly_pushed && ((key == _activeKey && _previousKey) == 0 || (key == _previousKey))) {
        saved_releases.push_back(key);
        return;
    }

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

bool MovementController::isReady() const {
    auto now = std::chrono::high_resolution_clock::now();
    float diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if ((!expensiveLast && diff >= MOVEMENT_DELAY * 1000) || (expensiveLast && diff >= MOVEMENT_DELAY * 1000 * std::sqrt(2))) {
        last = now;
        return true;
    }
    return false;
}
