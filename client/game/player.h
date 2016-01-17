#ifndef PLAYER_H
#define PLAYER_H

class Player {

unsigned int _x;
unsigned int _y;

public:
    Player(unsigned int, unsigned int);
    ~Player();

    unsigned int x() const;
    unsigned int y() const;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
};

#endif // PLAYER_H
