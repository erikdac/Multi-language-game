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
unsigned int _mana;

public:
    Player();
    Player(std::string, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    virtual ~Player();

    std::string name() const;
    unsigned int x() const;
    unsigned int y() const;
    unsigned int level() const;
    unsigned int health() const;
    unsigned int max_health() const;
    unsigned int mana() const;
    unsigned int max_mana() const;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void update_health(int);

    bool operator==(const Player) const;

    unsigned int distance_to_player(const Player &) const;

    virtual void load_graphics() const override;

private:
    void sendMovement() const;
};

#endif // PLAYER_H
