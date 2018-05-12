#include "movementcontroller.h"

#include <chrono>
#include <cmath>
#include <QKeyEvent>
#include <algorithm>
#include <QtDebug>

MovementController::MovementController(const int movement_delay_millis)
    : _movement_delay_millis(movement_delay_millis) {

}

MovementController::~MovementController() {

}

void MovementController::execute(Self * const self) {
    if (!isReady() || _pushedKeys.empty()) {
        return;
    }

    int primeKey = _pushedKeys.back();
    int secondaryKey = 0;
    if (_pushedKeys.size() >= 2) {
        secondaryKey = _pushedKeys[_pushedKeys.size() - 2];
    }

    if ((primeKey == Qt::Key_W && secondaryKey == Qt::Key_A) || (primeKey == Qt::Key_A && secondaryKey == Qt::Key_W)) {
        self->moveUpLeft();
        _expensiveLast = true;
    }
    else if ((primeKey == Qt::Key_W && secondaryKey == Qt::Key_D) || (primeKey == Qt::Key_D && secondaryKey == Qt::Key_W)) {
        self->moveUpRight();
        _expensiveLast = true;
    }
    else if ((primeKey == Qt::Key_S && secondaryKey == Qt::Key_A) || (primeKey == Qt::Key_A && secondaryKey == Qt::Key_S)) {
        self->moveDownLeft();
        _expensiveLast = true;
    }
    else if ((primeKey == Qt::Key_S && secondaryKey == Qt::Key_D) || (primeKey == Qt::Key_D && secondaryKey == Qt::Key_S)) {
        self->moveDownRight();
        _expensiveLast = true;
    }
    else if (primeKey == Qt::Key_W) {
        self->moveUp();
        _expensiveLast = false;
    }
    else if (primeKey == Qt::Key_A) {
        self->moveLeft();
        _expensiveLast = false;
    }
    else if (primeKey == Qt::Key_S) {
        self->moveDown();
        _expensiveLast = false;
    }
    else if (primeKey == Qt::Key_D) {
        self->moveRight();
        _expensiveLast = false;
    } else {
        qWarning() << "Unknown key combination!";
    }

    _newlyPushed.clear();
    for (const int key : _savedReleases) {
        released(key);
    }
}

void MovementController::pushed(const int key) {
    _pushedKeys.push_back(key);
    _newlyPushed.push_back(key);
    _savedReleases.erase(std::remove(_savedReleases.begin(), _savedReleases.end(), key), _savedReleases.end());
}

void MovementController::released(const int key) {
    if(std::find(_newlyPushed.begin(), _newlyPushed.end(), key) != _newlyPushed.end()) {
        _savedReleases.push_back(key);
    } else {
        _pushedKeys.erase(std::remove(_pushedKeys.begin(), _pushedKeys.end(), key), _pushedKeys.end());
    }
}

void MovementController::clear() {
    _pushedKeys.clear();
    _newlyPushed.clear();
    _savedReleases.clear();
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
