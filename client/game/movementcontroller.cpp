#include "movementcontroller.h"
#include "game/map.h"
#include "objects/self.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

static constexpr int SLEEP_TIME = 250 * 1000;

bool _isRunning;

char active_key;

typedef void (Self::*function)();

void run(const char key) {
    active_key = key;

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

char previousKey;

void stop() {
    _isRunning = false;
    if(t && t->joinable()) {
        t->join();
    }
}

static std::mutex key_mutex;

void movement_controller::start(const char key) {
    key_mutex.lock();
    stop();
    previousKey = active_key;
    _isRunning = true;
    t = new std::thread(run, key);
//    t->detach();
    key_mutex.unlock();
}

void movement_controller::released(const char key) {
    if (key == active_key && previousKey != 0) {
        // MAY CAUSE RACE-CONDITION!
        start(previousKey);
        key_mutex.lock();
        previousKey = 0;
        key_mutex.unlock();
    }
    else if (key == active_key) {
        key_mutex.lock();
        stop();
        active_key = 0;
        key_mutex.unlock();
    }
    else if (key == previousKey) {
        key_mutex.lock();
        previousKey = 0;
        key_mutex.unlock();
    }
}
