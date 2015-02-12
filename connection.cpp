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

void readInput() {

    fd_set readfds;     // Set of socket descriptors for select
    struct timeval tv;  // Timeout value

    while(!finished) {
        int received = 0;
        while (received < MAX_LINE_LENGTH) {
            FD_ZERO(&readfds);    // Erase the set of socket descriptors
            FD_SET(s0, &readfds); // Add the socket "s0" to the set

            tv = {0, 100000};   // Timeout value to for the reading to finish.

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

void disconnect() {
    finished = 1;
    shutdown(s0, 2);
    close(s0);
}

void sigHandler(int sigID) {
    printf("The SIGPIPE signal (connection is broken).\n");
    finished = 1;
}

void connectToServer() {

    // Set signal handler for the "SIGPIPE" signal
    // (used to intercept the signal about broken connection).
    if (signal(SIGPIPE, &sigHandler) == SIG_ERR) {
        perror("Cannot install a signal handler");
        exit(1);
    }

    // Create a socket
    s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        perror("Cannot create a socket"); exit(1);
    }

    // Fill in the address of server
    struct sockaddr_in peeraddr;
    int peeraddr_len;
    memset(&peeraddr, 0, sizeof(peeraddr));
    const char* peerHost = "localhost";

    // Resolve the server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL) {
        perror("Cannot define host address"); exit(1);
    }
    peeraddr.sin_family = AF_INET;
    short peerPort = 1337;

    peeraddr.sin_port = htons(peerPort);

    // Print a resolved address of server (the first IP of the host)
    printf(
        "peer addr = %d.%d.%d.%d, port %d\n",
        host->h_addr_list[0][0] & 0xff,
        host->h_addr_list[0][1] & 0xff,
        host->h_addr_list[0][2] & 0xff,
        host->h_addr_list[0][3] & 0xff,
        (int) peerPort
    );

    // Write resolved IP address of a server to the address structure
    memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to a remote server
    int res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
    if (res < 0) {
        perror("Cannot connect"); exit(1);
    }
    printf("Connected. Type a message and press \"Enter\".\n");

    // Define socket as non-blocking
    // res = fcntl(s0, F_SETFL, O_NONBLOCK);
    // if (res < 0) {
    //     perror("Cannot set a socket as non-blocking");
    //     exit(1);
    // }
}
