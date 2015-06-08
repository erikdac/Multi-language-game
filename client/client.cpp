#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "login.hpp"
#include "json11/json11.hpp"

using namespace json11;
using std::string;

int main2() {

    connectToServer();

    string input;
    while (true) {
        std::getline (std::cin,input);
        Json data = Json::object {
            {"Text", input}
        };
        output(data);
    }

    disconnect();
    return 0;
}
