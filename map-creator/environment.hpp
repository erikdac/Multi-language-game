#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>

class Environment {
public:
	enum Type {GRASS, WATER};

private:
	int _x;
	int _y;
	Type _type;

public:
	Environment(const Type type, const int x, const int y);
	int x() const;
	int y() const;
	Type type() const;
	std::string to_string() const;
};

#endif
