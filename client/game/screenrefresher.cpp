#include "screenrefresher.h"

ScreenRefresher::ScreenRefresher() : _isRunning(true) {

}

ScreenRefresher::~ScreenRefresher() {

}

void ScreenRefresher::run() {
    while (_isRunning) {
        emit repaint();
        usleep(40000);
    }
}

void ScreenRefresher::stop() {
    _isRunning = false;
}
