#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include <thread>
#include <mutex>

class MovementController {

    char _activeKey;
    char _previousKey;

public:
    void execute();
    void pushed(const char);
    void released(const char);
    void clear();

private:
    bool isReady() const;
};

#endif // MOVEMENTCONTROLLER_H
