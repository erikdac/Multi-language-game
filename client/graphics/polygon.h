#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

class Vertex {
	float _x;
	float _y;
    float _z;

public:
    Vertex(float x, float y, float z);

	float x() const;
	float y() const;
    float z() const;
};

class Color {
	int _r, _g, _b;

public:
	Color(int r, int g, int b);

	int r() const;
	int g() const;
	int b() const;
};

class Polygon {

	Color _color;
    std::vector<Vertex> _verticies;

public:
    Polygon(Color, std::vector<Vertex>);

    Color color() const;
    std::vector<Vertex> verticies() const;
};

#endif
