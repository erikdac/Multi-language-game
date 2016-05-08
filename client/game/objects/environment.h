#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "graphics.h"

class Environment : public Graphics {

protected:
    int _x;
    int _y;

public:
    Environment(const int x, const int y);
    virtual ~Environment();
    int x() const;
    int y() const;
};

#endif
