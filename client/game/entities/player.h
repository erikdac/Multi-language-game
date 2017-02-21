#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

#include <string>

class Player : public Actor {

public:
    Player();
    Player(std::string, int, int, int, int, int);
    virtual ~Player();

    virtual int max_health() const override;
    virtual int max_mana() const override;

    unsigned int distance_to_player(const Player &) const;

    virtual void update() override;
    virtual void draw() const override;
};

#endif // PLAYER_H
