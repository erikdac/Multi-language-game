#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "environment.hpp"

#define SIZE 50

typedef std::pair<int, std::vector<std::string>> sector;

Environment generateEnvironment() {
    int x = std::rand() % (SIZE * 10);
    int y = std::rand() % (SIZE * 10);
	Environment e(Environment::GRASS, x, y);
	return e;
}

sector generateSector() {
	int bytes = 0;
	sector s;
	for(int i = 0; i < std::rand() % (SIZE * SIZE); ++i) {
		Environment e = generateEnvironment();
		std::string str = e.to_string();
		s.second.push_back(str);
		bytes += str.size();
	}
	s.first = bytes;
	return s;
}

std::string generateOffsetString(int length) {
	std::string str = "";
	str.resize(length, '0');
	return str;
}

std::string offsetString(std::string offsetString, int value) {
	int offset = std::ceil(log10(value + 1));
	return offsetString.substr(offset);
}

void writeIndexes(std::ofstream & file, int bytes[]) {
	int offset = std::ceil(log10(bytes[SIZE * SIZE] + 1));
	std::string str = generateOffsetString(offset);
	file << offsetString(str, offset) << (offset + 1) << '\n';

	int startIndex = (offset + 1) * (SIZE * SIZE + 2);
	for(int i = 0; i <= SIZE * SIZE; ++i) {
		file << offsetString(str, startIndex + bytes[i]) << (startIndex + bytes[i]) << '\n';
	}
}

void writeSectors(std::ofstream & file, std::vector<std::string> map[]) {
	for(int i = 0; i < SIZE * SIZE; ++i) {
		for(std::string element : map[i]) {
			file << element << '\n';
		}
	}
}

int main() {
	int bytes[SIZE*SIZE + 1];
	bytes[0] = 0;
	std::vector<std::string> map[SIZE*SIZE];
	for(int i = 0; i < SIZE * SIZE; ++i) {
		sector s = generateSector();
		bytes[i+1] = bytes[i] + s.first;
		map[i] = s.second;
	}
	std::ofstream file;
	file.open("map.mf");
	
	std::srand(std::time(NULL));	// For random generating the environment.
	writeIndexes(file, bytes);
	writeSectors(file, map);
	
	file.close();
}
