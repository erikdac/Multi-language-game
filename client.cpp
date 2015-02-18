#include <iostream>
#include <stdio.h>
#include <string.h>
#include "connection.h"
#include "login.h"
#include "packages.h"

int main(int argc, char *argv[]) {

    // Connects to the server.   
    connectToServer();

    // Login to the server.
    login();
    
    std::string input;
    while (true) {
        std::cin >> input;
        Text username(input);
        Data * data = &username;
        output(data);
    }

    disconnect();
    return 0;
}
