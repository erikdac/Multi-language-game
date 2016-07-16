#ifndef CREATURE_HPP
#define CREATURE_HPP

class Creature {
public:
	enum Type {TROLL};

private:
	int _x;
	int _y;
	Type _type;

public:
	Creature(const Type type, const int x, const int y);
	std::string to_string() const;
};

#endif
