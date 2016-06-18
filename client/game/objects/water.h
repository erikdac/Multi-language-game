#ifndef WATER_H
#define WATER_H

#include "environment.h"

class Water : public Environment {

public:
    using Environment::Environment;
    virtual ~Water();

    virtual void load_graphics() const override;
};

#endif // WATER_H
