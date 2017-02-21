#ifndef SELF_H
#define SELF_H

#include "player.h"

class Self : public Player {

public:
    Self();
    Self(const Player &);
    virtual ~Self();

    float visualX() const {
        return _visualX;
    }

    float visualY() const {
        return _visualY;
    }

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveUpRight();
    void moveUpLeft();
    void moveDownRight();
    void moveDownLeft();

    void set_health(const int);
    void fix_position(const int, const int);

    virtual void update() override;
    virtual void draw() const override;

private:
    void set_position(const int, const int);
    void sendMovement() const;
};

#endif // SELF_H
