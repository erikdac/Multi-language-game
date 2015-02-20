#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <mutex>

using std::string;

extern std::mutex login_mutex;

void login();

#endif
