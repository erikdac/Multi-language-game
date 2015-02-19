#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <mutex>
#include "packages.h"

extern int s0;

bool online();
void connectToServer();
void readInput();
void sigHandler(int sigID);
void disconnect();

static std::mutex output_mutex;

template <typename T> void output(T d) {

    Text * t = dynamic_cast<Text*>(&d);

    char data[t->text.size()];
    strcpy(data, t->text.c_str());
    int buffer = strlen(data) + 1;

    if (buffer > 0) {
    	output_mutex.lock();
        int res = write(s0, data, buffer);
    	output_mutex.unlock();

        if (res < 0) {
            if (errno != EAGAIN)
                perror("Write error");
            else
                perror("Incompleted send");

        } else if (res == 0) {
            std::cout << "Connection closed" << std::endl;
        }
    }
}

#endif
