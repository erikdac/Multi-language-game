#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "connection.h"


const int MAX_LINE_LENGTH = 65534;

int main(int argc, char *argv[]) {

    // Connects to the server.   
    connectToServer();

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
