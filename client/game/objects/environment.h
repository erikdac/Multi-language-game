#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "graphics.h"

#include <string>

class Environment : Graphics {

private:
    int _x;
    int _y;

public:
    Environment(const int x, const int y);
    int x() const;
    int y() const;

    virtual void load_graphics() const override;
};

#endif
