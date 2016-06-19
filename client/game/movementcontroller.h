#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include <thread>
#include <mutex>

class MovementController {

    bool _isRunning;

    char _activeKey;
    char _previousKey;

    std::thread _t;
    std::mutex _key_mutex;

public:
    void pushed(const char);
    void released(const char);
    void clear();

private:
    void start(const char);
    void stop();
    void run(const char);
};

#endif // MOVEMENTCONTROLLER_H
