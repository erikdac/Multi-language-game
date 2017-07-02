#ifndef CONNECTION_H
#define CONNECTION_H

#include "json11/json11.hpp"

#include <QTcpSocket>
#include <vector>
#include <string>

namespace connection {
    void run(const std::string &);
    void write(const json11::Json &);
    std::vector<json11::Json> read();
    void disconnect();
    std::string authenticate(const std::string &, const int, const json11::Json &);
}

#endif
