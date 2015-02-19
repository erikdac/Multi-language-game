#include <iostream>
#include <string.h>
#include "connection.h"
#include "login.h"
#include "packages.h"

std::mutex login_mutex;

Text getInput() {
    std::string input;
    std::getline (std::cin,input);
    Text text(input);
    return text;
}

/*
template<typename T>
void testing(T data) {
//    Data * p = &d;
    Text* t = dynamic_cast<Text*>(&data);
    if(t) {
        std::cout << t->text << std::endl;
    } else {
        printf("WTF");
    }
}
*/
void login() {
    login_mutex.lock();

    do {
        std::cout << "Username: ";
        Text username = getInput();
        output(username);

        std::cout << "Password: ";
        Text password = getInput();
        output(password);

        login_mutex.lock();

    } while(!online());
}
