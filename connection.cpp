
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <thread>
#include "connection.h"

const int MAX_LINE_LENGTH = 65534;

static char readBuffer[MAX_LINE_LENGTH + 2];  // Read buffer

static char writeBuffer[MAX_LINE_LENGTH + 2]; // Write buffer
static char *writePos = writeBuffer;          // current position
static int writeBufferLen = 0;                // write buffer length

static int finished = 0;            // Finish the program

static int res;

Connection::Connection(int s0, int _res) {
	this->s0 = s0;
	res = _res;
}

void Connection::readInput() {

    fd_set readfds;     // Set of socket descriptors for select
    struct timeval tv;  // Timeout value

    while(!finished) {
        int received = 0;
        while (received < MAX_LINE_LENGTH) {
            FD_ZERO(&readfds);    // Erase the set of socket descriptors
            FD_SET(s0, &readfds); // Add the socket "s0" to the set

            tv.tv_sec = 0;        // Define the timeout value
            tv.tv_usec = 100000;  // 0.1 sec (in microseconds)

            //=== "select" is the key poit of the program! ============
            res = select(
                s0 + 1,   // Max. number of socket in all sets + 1
                &readfds, // Set of socket descriptors for reading
                NULL,     // Set of sockets for writing -- not used
                NULL,     // Set of sockets with exceptions -- not used
                &tv       // Timeout value
            );
            //=========================================================

            if (res < 0) {
                perror("Select error");
                finished = 1;
                break;
            } else if (res == 0) {
                break;  // No data is availible yet
            }

            // At this point, we can read an incoming data
            assert(FD_ISSET(s0, &readfds));

            res = read(
                s0,
                readBuffer + received,
                MAX_LINE_LENGTH - received
            );
            if (res > 0) {
                received += res;
            } else {
                if (res < 0 && errno != EAGAIN) {
                    perror("Read error");
                    finished = 1;
                }
                break;
            }
        }

        // Prints out the data.
        if (received > 0) {
            readBuffer[received] = 0;
            fprintf(stderr, "Received %d bytes:\n%s", received, readBuffer);
        }
    }
}
