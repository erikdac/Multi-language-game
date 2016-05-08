#ifndef GRAPHICS_H
#define GRAPHICS_H

static constexpr float VIEW_WIDTH = 12 + 0.5f;
static constexpr float VIEW_HEIGHT = 8 + 0.5f;

class Graphics {

public:
    virtual void load_graphics() const = 0;
};

#endif // GRAPHICS_H
