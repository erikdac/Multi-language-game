#include <iostream>
#include <vector>
#include <fstream>

#include "environment.hpp"

#define SIZE 10

typedef std::pair<int, std::vector<Environment>> sector;

Environment generateEnvironment() {
	Environment e(Environment::GRASS, 1, 2);
	return e;
}

std::string environmentToString(Environment & e) {
	std::string str = "";
	switch(e.type()) {
		case Environment::GRASS:
			str = "grass";
			break;
		case Environment::WATER:
			str = "water";
			break;
	}
	return str + " " + std::to_string(e.x()) + " " + std::to_string(e.y()) + "\n";
}

sector generateSector() {
	int bytes = 0;
	sector s;
	for(int i = 0; i < 100; ++i) {
		Environment e = generateEnvironment();
		s.second.push_back(e);
	}
	s.first = bytes;
	return s;
}

int main() {
	int bytes[SIZE*SIZE + 1];
	bytes[0] = 0;
	std::vector<Environment> map[SIZE*SIZE];
	for(int i = 0; i < SIZE * SIZE; ++i) {
		sector s = generateSector();
		bytes[i+1] = bytes[i] + s.first;
		map[i] = s.second;
	}
	std::ofstream file;
	file.open("map.mf");
	for(int i = 0; i <= SIZE * SIZE; ++i) {
		file << bytes[i];
	}
	file.close();
}
