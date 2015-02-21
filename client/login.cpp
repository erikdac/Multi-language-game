#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "login.hpp"
#include "json11/json11.h"

using std::string;

std::mutex login_mutex;

void login() {
    login_mutex.lock();
    Json data;
    string input;

    do {
        std::cout << "Username: ";
        std::getline (std::cin,input);
        data.set("Username", input);

        std::cout << "Password: ";
        std::getline (std::cin,input);
        data.set("Password", input);

        output(data);
        login_mutex.lock();

    } while(!online());
}
