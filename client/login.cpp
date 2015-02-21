#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "login.hpp"
#include "json11/json11.hpp"

using namespace json11;
using std::string;

std::mutex login_mutex;

void login() {
    login_mutex.lock();

    do {
        std::cout << "Username: ";
        string username;
        std::getline (std::cin, username);

        std::cout << "Password: ";
        string password;
        std::getline (std::cin, password);

        Json data = Json::object {
            {"Username", username},
            {"Password", password}
        };

        output(data);
        login_mutex.lock();

    } while(!online());
}
