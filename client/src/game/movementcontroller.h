#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "entities/self.h"

class MovementController {

    int _activeKey;
    int _previousKey;

public:
    void execute(Self * const);
    void pushed(const int);
    void released(const int);
    void clear();

private:
    bool isReady() const;
};

#endif // MOVEMENTCONTROLLER_H
