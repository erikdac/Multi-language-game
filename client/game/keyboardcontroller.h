#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "game/player.h"

#include <QThread>

class KeyboardController : public QThread {

    Q_OBJECT

public:
    KeyboardController(Player *);
    ~KeyboardController();
    void stop();
    void run();

private:
    Player * _player;
    bool _isRunning;
    void *fptr;


signals:
    void animate();
};

#endif // KEYBOARDCONTROLLER_H
