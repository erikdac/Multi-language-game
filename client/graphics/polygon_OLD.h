#ifndef POLYGON_OLD_H
#define POLYGON_OLD_H

#include <vector>

class Vertex {
    float _x;
    float _y;
    float _z;

public:
    Vertex();
    Vertex(float x, float y, float z);

    float x() const;
    float y() const;
    float z() const;
};

#endif
