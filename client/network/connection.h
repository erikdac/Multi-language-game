#ifndef CONNECTION_H
#define CONNECTION_H

#include "external/json11/json11.hpp"
#include "game/eventhandler.h"

namespace connection {
    extern EventHandler<json11::Json> _inputHandler;

    bool output(const json11::Json);
    void startReading();
    std::string readPacket(const int);
    void disconnect();
}

#endif
