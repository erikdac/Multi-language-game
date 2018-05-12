#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
namespace graphics {
    static constexpr float VIEW_WIDTH = 12 + 0.5f;
    static constexpr float VIEW_HEIGHT = 8 + 0.5f;
    static const int RGB_MAX = 255;

    class Graphics {

    protected:
        std::vector<float> relativePos(const float, const float, const float, const float) const;

    public:
        virtual ~Graphics();

        virtual void update(float ) = 0;
        virtual void draw() const = 0;
    };
}

#endif // GRAPHICS_H
