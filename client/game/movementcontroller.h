#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "entities/self.h"

class MovementController {

    char _activeKey;
    char _previousKey;

public:
    void execute(Self * const);
    void pushed(const char);
    void released(const char);
    void clear();

private:
    bool isReady() const;
};

#endif // MOVEMENTCONTROLLER_H
