#ifndef PACKAGES_H
#define PACKAGES_H

#include <string>

struct Data {
	virtual ~Data() {};
};

struct Text : Data {
    std::string text;
    Text(std::string text) : text(text) {;}
};

#endif
