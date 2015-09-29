#include "keyboardcontroller.h"

#include <iostream>

KeyboardController::KeyboardController(Player * player)
    : _isRunning(true)
    , _player(player)
{

}

KeyboardController::~KeyboardController() {

}

void KeyboardController::stop() {
    _isRunning = false;
}

void KeyboardController::run() {
    while(_isRunning) {
        _player->moveForward();
        _player->turnLeft();
        emit animate();
        usleep(50000);
    }
}
