#ifndef CONNECTION_H
#define CONNECTION_H

void connectToServer();
void readInput();
void output(char data[]);
void sigHandler(int sigID);
void disconnect();

#endif
