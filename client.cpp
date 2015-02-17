#include <iostream>
#include <stdio.h>
#include "connection.h"
#include "packages.h"

int main(int argc, char *argv[]) {

    // Connects to the server.   
    connectToServer();

    
    char writeBuffer[BUFFER_SIZE + 2];
    while (true) {
        if (fgets(writeBuffer, BUFFER_SIZE, stdin) != NULL) {
            Message m(writeBuffer);
            Data * data = &m;
            output(data);
        }
    }

    disconnect();
    return 0;
}
