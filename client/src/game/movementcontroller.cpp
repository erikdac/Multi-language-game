#include "movementcontroller.h"
#include "config.h"

#include <chrono>
#include <cmath>
#include <vector>
#include <QKeyEvent>

bool newly_pushed = false;
std::vector<int> saved_releases;
bool expensiveLast = false;

void MovementController::execute(Self * const self) {
    if (!isReady() || _activeKey == 0) {
        return;
    }

    if ((_activeKey == Qt::Key_W && _previousKey == Qt::Key_A) || (_activeKey == Qt::Key_A && _previousKey == Qt::Key_W)) {
        self->moveUpLeft();
        expensiveLast = true;
    }
    else if ((_activeKey == Qt::Key_W && _previousKey == Qt::Key_D) || (_activeKey == Qt::Key_D && _previousKey == Qt::Key_W)) {
        self->moveUpRight();
        expensiveLast = true;
    }
    else if ((_activeKey == Qt::Key_S && _previousKey == Qt::Key_A) || (_activeKey == Qt::Key_A && _previousKey == Qt::Key_S)) {
        self->moveDownLeft();
        expensiveLast = true;
    }
    else if ((_activeKey == Qt::Key_S && _previousKey == Qt::Key_D) || (_activeKey == Qt::Key_D && _previousKey == Qt::Key_S)) {
        self->moveDownRight();
        expensiveLast = true;
    }
    else if (_activeKey == Qt::Key_W) {
        self->moveUp();
        expensiveLast = false;
    }
    else if (_activeKey == Qt::Key_A) {
        self->moveLeft();
        expensiveLast = false;
    }
    else if (_activeKey == Qt::Key_S) {
        self->moveDown();
        expensiveLast = false;
    }
    else if (_activeKey == Qt::Key_D) {
        self->moveRight();
        expensiveLast = false;
    }

    if (newly_pushed) {
        newly_pushed = false;
        for (const int k : saved_releases) {
            released(k);
        }
        saved_releases.clear();
    }
}

void MovementController::pushed(const int key) {
    _previousKey = _activeKey;
    _activeKey = key;
    newly_pushed = true;
}

void MovementController::released(const int key) {
    if (newly_pushed && ((key == _activeKey && _previousKey == 0) || (key == _previousKey))) {
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
    if ((!expensiveLast && diff >= MOVEMENT_DELAY_MILLIS) || (expensiveLast && diff >= MOVEMENT_DELAY_MILLIS * std::sqrt(2))) {
        last = now;
        return true;
    }
    return false;
}
