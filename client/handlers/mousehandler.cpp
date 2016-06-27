#include "mousehandler.h"

MouseHandler::MouseHandler() {

}

MouseHandler::~MouseHandler() {
    // Do you need to delete QEvents?
    _events.clear();
}

void MouseHandler::addEvent(QMouseEvent * event) {
    _mutex.lock();
    _events.push_back(event);
    _mutex.unlock();
}

std::vector<QMouseEvent *> MouseHandler::events() {
    _mutex.lock();
    std::vector<QMouseEvent *> copy = _events;
    _events.clear();
    _mutex.unlock();
    return copy;
}
