#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include <QThread>

typedef void (*func_t)();

class KeyboardController : public QThread {
    Q_OBJECT

    char _key;
    bool _isRunning;

public:
    KeyboardController(char);
    void stop();
    void run();
};

#endif // KEYBOARDCONTROLLER_H
