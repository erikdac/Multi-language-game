#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include "ui/loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

extern int s0;

namespace connection {
    void setActiveWidget(QWidget *);
    bool connectToServer();
    bool output(const Json);
    void disconnect();
}

#endif
