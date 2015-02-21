#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "json11/json11.h"

extern int s0;

bool online();
void connectToServer();
void readInput();
void output(Json object);
void sigHandler(int sigID);
void disconnect();

#endif
