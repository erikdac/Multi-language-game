#ifndef ACTOR_H
#define ACTOR_H

#include "graphics/graphics.h"

#include <string>

class Actor : public graphics::Graphics {

protected:
    std::string _name;
    int _x;
    int _y;
    float _visualX;
    float _visualY;
    int _level;
    int _health;
    int _mana;

public:
    virtual ~Actor() {}

    std::string name() const;
    int x() const;
    int y() const;
    int level() const;
    int health() const;
    int mana() const;

    virtual int max_health() const = 0;
    virtual int max_mana() const = 0;
};

#endif // ACTOR_H
