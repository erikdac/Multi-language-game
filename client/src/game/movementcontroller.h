#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "entities/self.h"

class MovementController {

    int _activeKey = 0;
    int _previousKey = 0;
    bool _expensiveLast = false;
    int _movement_delay_millis;

public:
    MovementController(const int);
    ~MovementController();

    void execute(Self * const);
    void pushed(const int);
    void released(const int);
    void clear();

private:
    bool isReady() const;
};

#endif // MOVEMENTCONTROLLER_H
