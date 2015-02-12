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

const int MAX_LINE_LENGTH = 65534;

static char readBuffer[MAX_LINE_LENGTH + 2];  // Read buffer

static int finished = 0;            // Finish the program

static int s0;
static int res;

void connectToServer(int _s0, int _res) {
    s0 = _s0;
    res = _res;
}

void readInput() {

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

void output(char data[]) {

    // SET MUTEX-LOCK!

    int buffer = strlen(data) + 1;
    char *writePos = data;
    if (!finished && buffer > 0) {
        res = write(s0, writePos, buffer);

        if (res < 0) {
            if (errno != EAGAIN) {
                perror("Write error");
                return;              // Write error
            } else {
                perror("Incompleted send");
            }
        } else if (res == 0) {
            printf("Connection closed");
            return;
        } else if (res > 0) {
            writePos += res;
            buffer -= res;
        }
    }
    // Sends a "null"-byte
    res = write(s0, 0, 1);
}
