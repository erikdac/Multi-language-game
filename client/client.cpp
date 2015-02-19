#include <iostream>
#include <string.h>
#include "connection.h"
#include "login.h"
#include "packages.h"



int main(int argc, char *argv[]) {

    connectToServer();

    login();
    
    std::string input;
    while (true) {
        std::getline (std::cin,input);
        Text data(input);
        output(data);
    }

    disconnect();
    return 0;
}
