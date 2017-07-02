#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include "entities/self.h"

#include <vector>

class MovementController {

    std::vector<int> _pushedKeys;
    std::vector<int> _newlyPushed;
    std::vector<int> _savedReleases;
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
