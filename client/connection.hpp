#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "json11.hpp"

extern int s0;

bool online();
void connectToServer();
void readInput();
void output(json11::Json object);
void sigHandler(int sigID);
void disconnect();

#endif
