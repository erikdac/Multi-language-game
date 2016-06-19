#include "movementcontroller.h"
#include "game/map.h"
#include "objects/self.h"

#include <unistd.h>

static constexpr int SLEEP_TIME = 250 * 1000;

typedef void (Self::*function)();

void MovementController::run(const char key) {

    function fp;
    switch (key) {
        case 'w': fp = &Self::moveUp; break;
        case 'a': fp = &Self::moveLeft; break;
        case 's': fp = &Self::moveDown; break;
        case 'd': fp = &Self::moveRight; break;
        default: return;
    }

    while (_isRunning) {
        (_self->*fp)();
        usleep(SLEEP_TIME);
    }
}

void MovementController::stop() {
    _isRunning = false;
    _activeKey = 0;
    if(_t.joinable()) {
        _t.join();
    }
}

void MovementController::start(const char key) {
    _previousKey = _activeKey;
    stop();
    _activeKey = key;
    _isRunning = true;
    _t = std::thread(&MovementController::run, this, key);
}

void MovementController::pushed(const char key) {
    _key_mutex.lock();
    start(key);
    _key_mutex.unlock();
}

void MovementController::released(const char key) {
    _key_mutex.lock();
    if (key == _activeKey && _previousKey != 0) {
        start(_previousKey);
        _previousKey = 0;
    }
    else if (key == _activeKey) {
        stop();
    }
    else if (key == _previousKey) {
        _previousKey = 0;
    }
    _key_mutex.unlock();
}

void MovementController::clear() {
    stop();
    _activeKey = 0;
    _previousKey = 0;
}
