#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

#include <string>

class Player : public Actor {

public:
    Player(std::string, int, int, int, int, int);
    virtual ~Player();

    bool operator==(const Player &) const;

    virtual int max_health() const override;
    virtual int max_mana() const override;

    unsigned int distance_to_player(const Player &) const;

    virtual void load_graphics() const override;
};

#endif // PLAYER_H
