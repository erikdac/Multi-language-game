#include "movementcontroller.h"
#include "game/map.h"
#include "objects/self.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

static constexpr int SLEEP_TIME = 250 * 1000;

bool _isRunning;

char activeKey;
char previousKey;

typedef void (Self::*function)();

void run(const char key) {

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

static std::thread * t;

static std::mutex key_mutex;

void stop() {
    _isRunning = false;
    activeKey = 0;
    if(t && t->joinable()) {
        t->join();
    }
}

void start(const char key) {
    previousKey = activeKey;
    stop();
    activeKey = key;
    _isRunning = true;
    t = new std::thread(run, key);
}

void movement_controller::pushed(const char key) {
    key_mutex.lock();
    start(key);
    key_mutex.unlock();
}

void movement_controller::released(const char key) {
    key_mutex.lock();
    if (key == activeKey && previousKey != 0) {
        start(previousKey);
        previousKey = 0;
    }
    else if (key == activeKey) {
        stop();
    }
    else if (key == previousKey) {
        previousKey = 0;
    }
    key_mutex.unlock();
}
