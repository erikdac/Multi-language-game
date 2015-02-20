#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "login.hpp"

using namespace json11;
using std::string;

int main(int argc, char *argv[]) {

    connectToServer();

    login();
    
    string input;
    while (true) {
        std::getline (std::cin,input);
        Json data = Json::object {
            {"text", input},
        };
        output(data);
    }

    disconnect();
    return 0;
}
