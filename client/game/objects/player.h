#ifndef PLAYER_H
#define PLAYER_H

#include "graphics.h"

#include <string>

class Player : public Graphics {

protected:
    std::string _name;
    int _x;
    int _y;
    int _level;
    int _health;
    int _mana;

public:
    Player();
    Player(std::string, int, int, int, int, int);
    virtual ~Player();

    bool operator==(const Player &) const;

    std::string name() const;
    int x() const;
    int y() const;
    int level() const;
    int health() const;
    int max_health() const;
    int mana() const;
    int max_mana() const;

    unsigned int distance_to_player(const Player &) const;

    virtual void load_graphics() const override;
};

#endif // PLAYER_H
