#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

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
};

#endif
