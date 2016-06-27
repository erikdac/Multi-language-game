#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <vector>
#include <mutex>
#include <utility>
#include <QKeyEvent>

class KeyboardHandler {

    std::vector<std::pair<QKeyEvent, bool>> _events;
    std::mutex _mutex;

public:
    KeyboardHandler();
    ~KeyboardHandler();

    void addEvent(QKeyEvent, bool);
    std::vector<std::pair<QKeyEvent, bool>> events();
};

#endif // KEYBOARDHANDLER_H
