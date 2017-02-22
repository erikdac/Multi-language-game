#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "graphics/graphics.h"
#include "graphics/polygon.h"

#include <vector>

void initEnvironments();

class Environment : public Graphics {

    int _x;
    int _y;
    bool _walkable;
    std::vector<Polygon> * _polygons;

public:
	Environment();
    Environment(int, int, bool, std::vector<Polygon> *);
    virtual ~Environment();

    int x() const;
    int y() const;
    bool walkable() const;

    virtual void update(float) override;
    virtual void draw() const override;
};

#endif
