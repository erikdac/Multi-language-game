#ifndef CONNECTION_H
#define CONNECTION_H

#include "json11/json11.hpp"

using namespace json11;

namespace connection {
    bool output(const Json);
    std::string readPacket(const unsigned int);
    void disconnect();
}

#endif
