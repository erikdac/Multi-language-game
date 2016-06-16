#ifndef CONNECTION_H
#define CONNECTION_H

#include "ui/loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

namespace connection {
    void setActiveWidget(QWidget *);
    bool output(const Json);
    std::string readPacket();
    void disconnect();
}

#endif
