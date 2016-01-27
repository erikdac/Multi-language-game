#ifndef PLAYER_H
#define PLAYER_H

#include "graphics.h"

#include <string>

class Player : public Graphics {

std::string _name;
unsigned int _x;
unsigned int _y;
unsigned int _level;
unsigned int _health;

public:
    Player(std::string, unsigned int, unsigned int, unsigned int, unsigned int);
    virtual ~Player();

    std::string name() const;
    unsigned int x() const;
    unsigned int y() const;
    unsigned int level() const;
    unsigned int health() const;
    unsigned int max_health() const;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    virtual void load_graphics() override;

private:
    void sendMovement();
};

#endif // PLAYER_H
