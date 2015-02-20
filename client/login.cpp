#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "login.hpp"

using namespace json11;
using std::string;

std::mutex login_mutex;

void login() {
    login_mutex.lock();
    string input;

    do {
        std::cout << "Username: ";
        std::getline (std::cin,input);
        Json username = Json::object {
            {"text", input},
        };
        output(username);

        std::cout << "Password: ";
        std::getline (std::cin,input);
        Json password = Json::object {
            {"text", input},
        };
        output(password);

        login_mutex.lock();

    } while(!online());
}
