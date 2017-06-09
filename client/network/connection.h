#ifndef CONNECTION_H
#define CONNECTION_H

#include "external/json11/json11.hpp"

namespace connection {
    bool output(const json11::Json);
    std::string readPacket(const int);
    void disconnect();
}

#endif
