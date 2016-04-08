#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

#include "environment.hpp"

#define SIZE 2

typedef std::pair<int, std::vector<std::string>> sector;

Environment generateEnvironment() {
	Environment e(Environment::GRASS, 1, 2);
	return e;
}

sector generateSector() {
	int bytes = 0;
	sector s;
	for(int i = 0; i < 100; ++i) {
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
	str.resize(length, ' ');
	return str;
}

std::string offsetString(std::string offsetString, int value) {
	int offset = std::ceil(log10(value + 1));
	return offsetString.substr(offset);
}

void writeIndexes(std::ofstream & file, int bytes[]) {
	int offset = std::ceil(log10(bytes[SIZE * SIZE] + 1));
	std::string str = generateOffsetString(offset);
	file << offset << offsetString(str, offset) << "\n";

	int startIndex = (offset + 1) * (SIZE * SIZE + 2);
	for(int i = 0; i <= SIZE * SIZE; ++i) {
		file << (startIndex + bytes[i]) << offsetString(str, startIndex + bytes[i]) << "\n";
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
	
	writeIndexes(file, bytes);
	
	file.close();
}
