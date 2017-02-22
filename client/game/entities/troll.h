#ifndef TROLL_H
#define TROLL_H

#include "actor.h"

#include <string>

class Troll : public Actor {

public:
    Troll(std::string, int, int, int, int);
    virtual ~Troll();

    virtual int max_health() const override;
    virtual int max_mana() const override;

    virtual void update(float) override;
    virtual void draw() const override;
};

#endif // TROLL_H
