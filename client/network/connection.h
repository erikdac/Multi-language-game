#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QThread>
#include "network/network_reader.h"
#include "loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

extern int s0;

void setNetworkReader(Network_Reader * Network_Reader);
void setActiveWidget(LoginWidget * object);
bool online();
void connectToServer();
void readInput();
void output(const Json object);
void sigHandler(int sigID);
void disconnect();

#endif
