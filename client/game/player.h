#ifndef PLAYER_H
#define PLAYER_H


class Player {

public:
    Player(unsigned int, unsigned int, unsigned int, double);
    ~Player();

    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();

    // TODO: Remove
    void printForTest();

private:
    unsigned int _x;
    unsigned int _y;
    unsigned int _z;
    double _direction;

};

#endif // PLAYER_H
