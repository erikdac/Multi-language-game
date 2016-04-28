#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "graphics.h"

#include <string>

class Environment : Graphics {
public:
    enum Type {GRASS, WATER};

private:
    int _x;
    int _y;
    Type _type;

public:
    Environment(const std::string type, const int x, const int y);
    int x() const;
    int y() const;
    Type type() const;

    virtual void load_graphics() const override;
};

#endif
