#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "game/player.h"

#include <QThread>

typedef void (*func_t)();

class KeyboardController : public QThread {
    Q_OBJECT

    char _key;

public:
    KeyboardController(char);
    ~KeyboardController();
    void stop();
    void run();
};

#endif // KEYBOARDCONTROLLER_H
