#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "environment.hpp"

static const int MAP_X = 50;
static const int MAP_Y = 50;
static const int MAP_SECTOR_SIZE = 14;

typedef std::vector<Environment> sector;

Environment generateEnvironment(const unsigned int sectorX, const unsigned int sectorY) {
    const int x = (std::rand() % MAP_SECTOR_SIZE) + (MAP_SECTOR_SIZE * sectorX);
    const int y = (std::rand() % MAP_SECTOR_SIZE) + (MAP_SECTOR_SIZE * sectorY);
	return Environment(Environment::GRASS, x, y);
}

void removeDuplicateEnvironments(sector & old) {
	if(old.size() == 0) {
		return;
	}
 
	std::sort(old.begin(), old.end());
	sector s;
	s.push_back(old[0]);
	for (std::size_t i = 1; i < old.size(); ++i) {
		if (old[i] != old[i-1]) {
			s.push_back(old[i]);
		}
	}
	old = s;
}

sector generateSector(const unsigned int sectorX, const unsigned int sectorY) {
	sector s;
	int lim = std::sqrt(std::rand() % ((MAP_SECTOR_SIZE * MAP_SECTOR_SIZE) + MAP_SECTOR_SIZE)) + MAP_SECTOR_SIZE;
	for(int i = 0; i < lim; ++i) {
		Environment e = generateEnvironment(sectorX, sectorY);
		s.push_back(e);
	}
	removeDuplicateEnvironments(s);
	return s;
}

sector generateWaterSector(const unsigned int sectorX, const unsigned int sectorY) {
	sector s;
	const int fromX = MAP_SECTOR_SIZE * sectorX;
	const int fromY = MAP_SECTOR_SIZE * sectorY;
	for (int x = fromX; x < fromX + MAP_SECTOR_SIZE; ++x) {
		for (int y = fromY; y < fromY + MAP_SECTOR_SIZE; ++y) {
			Environment water(Environment::WATER, x, y);
			s.push_back(water);
		}
	}
	return s;
}

void writeToFile(std::ofstream & file, std::vector<sector> & sectors) {
	for (const sector & s : sectors) {
		file << s.size() << std::endl;		// Each section starts with its size.
		for (const Environment & e : s) {
			file << e.to_string() << std::endl;
		}
	}
}

int main() {
	std::vector<sector> sectors;
	srand(std::time(NULL));	// For random generating the environment.

	// Generate left water line
	for (unsigned int i = 0; i < MAP_Y; ++i) {
		sector s = generateWaterSector(0, i);
		sectors.push_back(s);
	}

	for(int i = 1; i < MAP_X-1; ++i) {
		sector left = generateWaterSector(i, 0);
		sectors.push_back(left);
		for(int j = 1; j < MAP_Y-1; ++j) {
			sector s = generateSector(i, j);
			sectors.push_back(s);
		}
		sector right = generateWaterSector(i, MAP_Y-1);
		sectors.push_back(right);
	}

	// Generate right water line
	for (unsigned int i = 0; i < MAP_Y; ++i) {
		sector s = generateWaterSector(MAP_X-1, i);
		sectors.push_back(s);
	}

	std::ofstream file;
	file.open("map.mf");
	writeToFile(file, sectors);
	file.close();
}
