#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <vector>
#include <mutex>

template <typename T>
class EventHandler {

    std::vector<T> _events;
    std::mutex _mutex;

public:
    EventHandler();
    ~EventHandler();

    void addEvent(T);
    std::vector<T> events();
};

template <typename T>
EventHandler<T>::EventHandler() {

}

template <typename T>
EventHandler<T>::~EventHandler() {
    _events.clear();
}

template <typename T>
void EventHandler<T>::addEvent(T event) {
    _mutex.lock();
    _events.push_back(event);
    _mutex.unlock();
}

template <typename T>
std::vector<T> EventHandler<T>::events() {
    _mutex.lock();
    std::vector<T> copy = _events;
    _events.clear();
    _mutex.unlock();
    return copy;
}

#endif // EVENTHANDLER_H
