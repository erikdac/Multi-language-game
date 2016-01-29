#include "keyboardcontroller.h"
#include "map.h"
#include "objects/player.h"

#include <unordered_map>

const int SLEEP_TIME = 250000;

bool _isRunning;

KeyboardController::KeyboardController(char key) : _key(key), _isRunning(true) {
}

KeyboardController::~KeyboardController() {

}

typedef void (Player::*function)();

void KeyboardController::run() {
    function fp;
    switch(_key) {
        case 'w': fp = &Player::moveUp; break;
        case 'a': fp = &Player::moveLeft; break;
        case 's': fp = &Player::moveDown; break;
        case 'd': fp = &Player::moveRight; break;
        default: return;
    }

    while(_isRunning) {
        (_self->*fp)();
        usleep(SLEEP_TIME);
    }
}

void KeyboardController::stop() {
    _isRunning = false;
}
