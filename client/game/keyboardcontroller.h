#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "game/player.h"

#include <QThread>

typedef void (*func_t)();

class KeyboardController : public QThread {

    Q_OBJECT

public:
    KeyboardController(Player *, char);
    ~KeyboardController();
    void stop();
    void run();

private:
    Player * _player;
    bool _isRunning;
    char _key;

};

#endif // KEYBOARDCONTROLLER_H
