#ifndef SELF_H
#define SELF_H

#include "player.h"

class Self : public Player {

public:
    Self(const Player *);
    virtual ~Self();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveUpRight();
    void moveUpLeft();
    void moveDownRight();
    void moveDownLeft();

    void set_health(const int);
    void set_position(const int, const int);

    virtual void load_graphics() const override;

private:
    void sendMovement() const;
};

#endif // SELF_H
