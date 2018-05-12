#include "graphics.h"

#include <cassert>

using namespace graphics;

Graphics::~Graphics() {

}

std::vector<float> Graphics::relativePos(const float x, const float y, const float xp, const float yp) const {
    std::vector<float> vec = {
        (-0.5f + x - xp)/VIEW_WIDTH,
        -(0.5f + y - yp)/VIEW_HEIGHT
    };
    assert(vec.size() == 2);
    return vec;
}
