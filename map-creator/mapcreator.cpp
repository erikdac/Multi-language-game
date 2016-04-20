#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "environment.hpp"

static const int MAP_X = 20;
static const int MAP_Y = 20;
static const int MAP_SECTOR_SIZE = 15;

typedef std::vector<Environment> sector;

Environment generateEnvironment(const unsigned int sectorX, const unsigned int sectorY) {
    int x = (std::rand() % MAP_SECTOR_SIZE) + (MAP_SECTOR_SIZE * sectorX);
    int y = (std::rand() % MAP_SECTOR_SIZE) + (MAP_SECTOR_SIZE * sectorY);
	return Environment(Environment::GRASS, x, y);
}

sector generateSector(const unsigned int sectorX, const unsigned int sectorY) {
	sector s;
	int lim = std::rand() % (MAP_SECTOR_SIZE * MAP_SECTOR_SIZE);
	for(int i = 0; i < lim; ++i) {
		Environment e = generateEnvironment(sectorX, sectorY);
		s.push_back(e);
	}
	return s;
}

void writeToFile(std::ofstream & file, std::vector<sector> & sectors) {
	file << MAP_SECTOR_SIZE << std::endl;
	for(int i = 0; i < sectors.size(); ++i) {
		for(Environment & e : sectors[i]) {
			file << e.to_string() << std::endl;
		}
	}
}

int main() {
	std::vector<sector> sectors;
	for(int i = 0; i < MAP_X; ++i) {
		for(int j = 0; j < MAP_Y; ++j) {
			sector s = generateSector(i, j);
			sectors.push_back(s);
		}
	}
	std::ofstream file;
	file.open("map.mf");
	
	srand(std::time(NULL));	// For random generating the environment.
	writeToFile(file, sectors);
	
	file.close();
}

