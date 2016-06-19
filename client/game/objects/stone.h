#ifndef STONE_H
#define STONE_H

#include "environment.h"

class Stone : public Environment {

public:
    using Environment::Environment;
    virtual ~Stone();

    virtual bool isWalkable() const override;

    virtual void load_graphics() const override;
};

#endif // STONE_H
