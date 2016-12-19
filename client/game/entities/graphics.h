#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <cassert>

static constexpr float VIEW_WIDTH = 12 + 0.5f;
static constexpr float VIEW_HEIGHT = 8 + 0.5f;

class Graphics {

public:
    virtual ~Graphics() {}

    virtual void update() = 0;
    virtual void draw() const = 0;

    std::vector<float> relativePos(const float x, const float y, const float xp, const float yp) const {
        std::vector<float> vec = {
            (-0.5f + x - xp)/VIEW_WIDTH,
            -(0.5f + y - yp)/VIEW_HEIGHT
        };
        assert(vec.size() == 2);
        return vec;
    }
};

#endif // GRAPHICS_H
