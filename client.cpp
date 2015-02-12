#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <thread>
#include "connection.h"

// Socket
const int MAX_LINE_LENGTH = 65534;

int main(int argc, char *argv[]) {
   
    connectToServer();

    // Starts reading data from server.
    std::thread reader(readInput);

    char writeBuffer[MAX_LINE_LENGTH + 2];

    while (true) {      
        printf("Message: ");

        if (fgets(writeBuffer, MAX_LINE_LENGTH, stdin) != NULL) {
            output(writeBuffer);
        } else {
            break;  // Break on Ctrl+D
        }
    }

    disconnect();
    return 0;
}
