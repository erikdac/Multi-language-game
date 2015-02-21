#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "login.hpp"
#include "json11/json11.h"

using std::string;

int main(int argc, char *argv[]) {

    connectToServer();

    login();
    
    string input;
    while (true) {
        std::getline (std::cin,input);
        Json data;
        data.set("text", input);
        output(data);
    }

    disconnect();
    return 0;
}
