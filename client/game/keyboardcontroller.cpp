#include "keyboardcontroller.h"
#include "map.h"
#include "player.h"

#include <iostream>

const int SLEEP_TIME = 200000;

KeyboardController::KeyboardController(char key) : _key(key) {

}

KeyboardController::~KeyboardController() {

}

void KeyboardController::run() {

    if(_key == 'w') {
        while(true) {
            _player->moveUp();
            usleep(SLEEP_TIME);
        }
    } else if(_key == 's') {
        while(true) {
            _player->moveDown();
            usleep(SLEEP_TIME);
        }
    } else if(_key == 'a') {
        while(true) {
            _player->moveLeft();
            usleep(SLEEP_TIME);
        }
    } else if(_key == 'd') {
        while(true) {
            _player->moveRight();
            usleep(SLEEP_TIME);
        }
    }
}
