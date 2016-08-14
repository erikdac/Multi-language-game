#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "environment.hpp"
#include "creature.hpp"

static const int MAP_X = 50;
static const int MAP_Y = 50;
static const int MAP_SLICE = 14;

Environment generateEnvironment(const int x, const int y) {
    int choice = std::rand() % 10;
    switch (choice) {
	case 0:
		return Environment(Environment::STONE, x, y);
	default:
		return Environment(Environment::GRASS, x, y);
    }
}

Creature generateCreature(const int x, const int y) {
	return Creature(Creature::TROLL, x, y);
}

void writeToFile(std::ofstream & file, std::vector<Environment> & vec) {
	for (const Environment & e : vec) {
		file << e.to_string() << std::endl;
	}
}

int main() {
	std::vector<Environment> environment_map;
	std::vector<Creature> creature_map;
	srand(std::time(NULL));	// For random generating the environment.

	// Generate left water line
	for (int x = 0; x < MAP_SLICE; ++x) {
		for (int y = 0; y < MAP_Y * MAP_SLICE; ++y) {
			Environment water(Environment::WATER, x, y);
			environment_map.push_back(water);
		}
	}

	for(int x = MAP_SLICE; x < MAP_X * MAP_SLICE - MAP_SLICE; ++x) {
		for(int y = 0; y < MAP_SLICE; ++y) {
			Environment water(Environment::WATER, x, y);
			environment_map.push_back(water);
		}

		for (int y = MAP_SLICE; y < MAP_Y * MAP_SLICE - MAP_SLICE; ++y) {
			Environment env = generateEnvironment(x, y);
			environment_map.push_back(env);
			if (std::rand() % 500 == 0) {
				Creature cr = generateCreature(x, y);
				creature_map.push_back(cr);
			}
		}

		for(int y =  MAP_Y * MAP_SLICE - MAP_SLICE; y < MAP_Y * MAP_SLICE; ++y) {
			Environment water(Environment::WATER, x, y);
			environment_map.push_back(water);
		}
	}

	// Generate right water line
	for (int x = MAP_X * MAP_SLICE - MAP_SLICE; x < MAP_X * MAP_SLICE; ++x) {
		for (int y = 0; y < MAP_Y * MAP_SLICE; ++y) {
			Environment water(Environment::WATER, x, y);
			environment_map.push_back(water);
		}
	}

	std::ofstream file;
	file.open("map.mf");
	for (const Environment & e : environment_map) {
		file << e.to_string() << std::endl;
	} 
	file.close();

	file.open("spawns.mf");
	for (const Creature & c : creature_map) {
		file << c.to_string() << std::endl;
	} 
	file.close();
}
