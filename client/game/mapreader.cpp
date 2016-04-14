#include "mapreader.h"
#include "objects/environment.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::pair<int, int> readIndexes(std::ifstream & file, int lineBytes, int sector) {
    file.seekg((sector + 1) * lineBytes);
    int start, stop;
    file >> start >> stop;
    std::pair<int, int> p(start, stop);
    return p;
}

std::vector<Environment> readSector(std::ifstream & file, int fromByte, int toByte) {
    std::vector<Environment> sector;
    file.seekg(fromByte);
    while(file.tellg() < toByte) {
        std::string str;
        int x, y;
        file >> str >> x >> y;
        Environment e(str, x, y);
        sector.push_back(e);
    }
    return sector;
}

std::vector<Environment> getSector(int sectorIndex) {
    std::ifstream file;
    file.open("map.mf");
    int lineBytes;
    file >> lineBytes;

    for(int i = 0; i < 50; ++i) {
        file.seekg(i);
        std::cout << file.peek() << std::endl;
    }

    std::pair<int, int> indexes = readIndexes(file, lineBytes, sectorIndex);
    std::cout << indexes.first << " : " << indexes.second << std::endl;
    std::vector<Environment> sector = readSector(file, indexes.first, indexes.second);

    file.close();

    return sector;
}
