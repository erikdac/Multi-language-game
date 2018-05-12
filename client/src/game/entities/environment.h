#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "graphics/graphics.h"
#include "graphics/polygon.h"

#include <vector>

class Environment : public graphics::Graphics {

    int _x;
    int _y;
    bool _walkable;
    std::vector<graphics::Polygon> * _polygons;

public:
	Environment();
    Environment(int, int, bool, std::vector<graphics::Polygon> *);
    virtual ~Environment();

    int x() const;
    int y() const;
    bool walkable() const;

    virtual void update(float) override;
    virtual void draw() const override;
};

#endif
