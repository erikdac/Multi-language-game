#ifndef CONNECTION_H
#define CONNECTION_H

#include "packages.h"

const int BUFFER_SIZE= 65534;

void connectToServer();
void readInput();
void output(Data * d);
void sigHandler(int sigID);
void disconnect();

#endif
