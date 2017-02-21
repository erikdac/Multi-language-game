#ifndef PARSER_HPP
#define PARSER_HPP

#include "external/json11/json11.hpp"
#include "entities/environment.h"

namespace parser {
    void initialize();
	Environment parseEnvironment(const json11::Json &);
}

#endif
