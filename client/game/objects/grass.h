#ifndef GRASS_H
#define GRASS_H

#include "environment.h"

class Grass : public Environment {

public:
    using Environment::Environment;

    virtual void load_graphics() const override;
};

#endif // GRASS_H
