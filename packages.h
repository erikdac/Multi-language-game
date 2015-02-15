#ifndef PACKAGES_H
#define PACKAGES_H

#include <string>

struct Data {
	virtual ~Data() {};
};

struct Message : Data {
    std::string message;
};

#endif
