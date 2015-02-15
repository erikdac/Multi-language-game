#ifndef CONNECTION_H
#define CONNECTION_H

#include "packages.h"

void connectToServer();
void readInput();
void output(Data * d);
void sigHandler(int sigID);
void disconnect();

#endif
