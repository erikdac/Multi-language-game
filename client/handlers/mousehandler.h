#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <vector>
#include <mutex>
#include <QMouseEvent>

class MouseHandler {

    std::vector<QMouseEvent *> _events;
    std::mutex _mutex;

public:
    MouseHandler();
    ~MouseHandler();

    void addEvent(QMouseEvent *);
    std::vector<QMouseEvent *> events();
};

#endif // MOUSEHANDLER_H
