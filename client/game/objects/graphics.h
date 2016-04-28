#ifndef GRAPHICS_H
#define GRAPHICS_H

const float VIEW_WIDTH = 12 + 0.5f;
const float VIEW_HEIGHT = 8 + 0.5f;

class Graphics {

public:
    Graphics() {}
    ~Graphics() {}

    virtual void load_graphics() const = 0;
};

#endif // GRAPHICS_H
