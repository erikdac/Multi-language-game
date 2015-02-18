#ifndef LOGIN_H
#define LOGIN_H

#include <mutex>
#include "packages.h"

extern std::mutex login_mutex;

Text getInput();
void login();

#endif
