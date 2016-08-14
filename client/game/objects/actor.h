#ifndef ACTOR
#define ACTOR

#include "graphics.h"

#include <string>

class Actor : public Graphics {

protected:
    std::string _name;
    int _x;
    int _y;
    double _direction;
    int _level;
    int _health;
    int _mana;

public:
    virtual ~Actor() {}

    std::string name() const {
        return _name;
    }

    int x() const {
        return _x;
    }

    int y() const {
        return _y;
    }

    double direction() const {
        return _direction;
    }

    int level() const {
        return _level;
    }

    int health() const {
        return _health;
    }

    int mana() const {
        return _mana;
    }

    virtual int max_health() const = 0;
    virtual int max_mana() const = 0;
    virtual void load_graphics() const = 0;
};

#endif // ACTOR
