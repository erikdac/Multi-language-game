#ifndef CONNECTION_H
#define CONNECTION_H

#include "external/json11/json11.hpp"

using namespace json11;

namespace connection {
    bool connectToServer();
    bool output(const Json);
    std::string readPacket(const int);
    void disconnect();
}

#endif
