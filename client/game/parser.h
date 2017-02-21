#ifndef PARSER_HPP
#define PARSER_HPP

#include "external/json11/json11.hpp"
#include "entities/environment.h"
#include "entities/player.h"
#include "entities/troll.h"

namespace parser {
    void initialize();
	Environment parseEnvironment(const json11::Json &);

    Player * parsePlayer(const json11::Json &);
    Troll * parseTroll(const json11::Json &);
}

#endif
