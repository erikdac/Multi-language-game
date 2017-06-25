#include "movementcontroller.h"

#include <chrono>
#include <cmath>
#include <vector>
#include <QKeyEvent>

bool newly_pushed = false;
std::vector<int> saved_releases;

MovementController::MovementController(const int movement_delay_millis)
    : _movement_delay_millis(movement_delay_millis) {

}

MovementController::~MovementController() {

}

void MovementController::execute(Self * const self) {
    if (!isReady() || _activeKey == 0) {
        return;
    }

    if ((_activeKey == Qt::Key_W && _previousKey == Qt::Key_A) || (_activeKey == Qt::Key_A && _previousKey == Qt::Key_W)) {
        self->moveUpLeft();
        _expensiveLast = true;
    }
    else if ((_activeKey == Qt::Key_W && _previousKey == Qt::Key_D) || (_activeKey == Qt::Key_D && _previousKey == Qt::Key_W)) {
        self->moveUpRight();
        _expensiveLast = true;
    }
    else if ((_activeKey == Qt::Key_S && _previousKey == Qt::Key_A) || (_activeKey == Qt::Key_A && _previousKey == Qt::Key_S)) {
        self->moveDownLeft();
        _expensiveLast = true;
    }
    else if ((_activeKey == Qt::Key_S && _previousKey == Qt::Key_D) || (_activeKey == Qt::Key_D && _previousKey == Qt::Key_S)) {
        self->moveDownRight();
        _expensiveLast = true;
    }
    else if (_activeKey == Qt::Key_W) {
        self->moveUp();
        _expensiveLast = false;
    }
    else if (_activeKey == Qt::Key_A) {
        self->moveLeft();
        _expensiveLast = false;
    }
    else if (_activeKey == Qt::Key_S) {
        self->moveDown();
        _expensiveLast = false;
    }
    else if (_activeKey == Qt::Key_D) {
        self->moveRight();
        _expensiveLast = false;
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
    if ((!_expensiveLast && diff >= _movement_delay_millis) || (_expensiveLast && diff >= _movement_delay_millis * std::sqrt(2))) {
        last = now;
        return true;
    }
    return false;
}
