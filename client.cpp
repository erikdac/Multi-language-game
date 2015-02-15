#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "connection.h"
#include "packages.h"

const int MAX_LINE_LENGTH = 65534;

int main(int argc, char *argv[]) {

    // Connects to the server.   
    connectToServer();

    char writeBuffer[MAX_LINE_LENGTH + 2];
    while (true) {      
        printf("Message: ");

        if (fgets(writeBuffer, MAX_LINE_LENGTH, stdin) != NULL) {
            Message m;
            m.message = writeBuffer;
            Data * data = &m;
            output(data);
        } else {
            break;  // Break on Ctrl+D
        }
    }

    disconnect();
    return 0;
}
