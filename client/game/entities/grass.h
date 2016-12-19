#ifndef GRASS_H
#define GRASS_H

#include "environment.h"

class Grass : public Environment {

public:
    using Environment::Environment;
    virtual ~Grass();

    virtual bool isWalkable() const override;

    virtual void update() override;
    virtual void draw() const override;
};

#endif // GRASS_H
