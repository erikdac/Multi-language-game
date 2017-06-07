#ifndef POLYGON_OLD2_H
#define POLYGON_OLD2_H

#include <vector>

class Color {
	int _r, _g, _b;

public:
    Color();
	Color(int r, int g, int b);

	int r() const;
	int g() const;
	int b() const;
};

#endif
