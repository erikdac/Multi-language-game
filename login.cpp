#include <iostream>
#include <string.h>
#include "connection.h"
#include "login.h"
#include "packages.h"

std::mutex login_mutex;

Text getInput() {
    std::string input;
    std::cin >> input;
    Text text(input);
    return text;
}

void login() {
    Data * data;

    login_mutex.lock();
    do {
        std::cout << "Username: ";
        Text username = getInput();
        data = &username;
        output(data);

        std::cout << "Password: ";
        Text password = getInput();
        data = &password;
        output(data);

        login_mutex.lock();
    } while(!online());
}
