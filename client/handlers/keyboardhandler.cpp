#include "keyboardhandler.h"

KeyboardHandler::KeyboardHandler() {

}

KeyboardHandler::~KeyboardHandler() {
    // Do you need to delete QEvents?
}

void KeyboardHandler::addEvent(QKeyEvent event, bool pushed) {
    _mutex.lock();
    std::pair<QKeyEvent, bool> p(event, pushed);
    _events.push_back(p);
    _mutex.unlock();
}

std::vector<std::pair<QKeyEvent, bool>> KeyboardHandler::events() {
    _mutex.lock();
    std::vector<std::pair<QKeyEvent, bool>> copy = _events;
    _events.clear();
    _mutex.unlock();
    return copy;
}
