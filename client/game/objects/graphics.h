#ifndef GRAPHICS_H
#define GRAPHICS_H

const int VIEW_WIDTH = 15;
const int VIEW_HEIGHT = 10;

class Graphics {

public:
    Graphics() {}
    ~Graphics() {}

    virtual void load_graphics() = 0;
};

#endif // GRAPHICS_H
