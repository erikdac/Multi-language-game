#ifndef PLAYER_H
#define PLAYER_H

#include "graphics.h"

class Player : public Graphics {

unsigned int _x;
unsigned int _y;

public:
    Player(unsigned int, unsigned int);
    virtual ~Player();

    unsigned int x() const;
    unsigned int y() const;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    virtual void load_graphics() override;

private:
    void sendMovement();
};

#endif // PLAYER_H
