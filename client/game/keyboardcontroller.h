#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "game/objects/player.h"

#include <QThread>

typedef void (*func_t)();

class KeyboardController : public QThread {
    Q_OBJECT

    char _key;
    bool _isRunning;

public:
    KeyboardController(char);
    ~KeyboardController();
    void stop();
    void run();
};

#endif // KEYBOARDCONTROLLER_H
