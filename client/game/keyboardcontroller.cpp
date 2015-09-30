#include "keyboardcontroller.h"

#include <iostream>

KeyboardController::KeyboardController(Player * player, char key)
    , _player(player)
    , _key(key)
{

}

KeyboardController::~KeyboardController() {

}

void KeyboardController::run() {
    if(_key == 'w') {
        while(true) {
            _player->moveForward();
            usleep(50000);
        }
    } else if(_key == 's') {
        while(true) {
            _player->moveBackward();
            usleep(50000);
        }
    } else if(_key == 'a') {
        while(true) {
            _player->turnLeft();
            usleep(50000);
        }
    } else if(_key == 'd') {
        while(true) {
            _player->turnRight();
            usleep(50000);
        }
    }
}
