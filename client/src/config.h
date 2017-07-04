#ifndef CONFIG
#define CONFIG

#include <string>

static const int MAX_UPDATE_RATE = 100;
static constexpr int MAX_FPS = 60 - 1;

static const std::string IP = "192.168.1.91";
static const int AUTHENTICATION_PORT = 1337;
static const int GAME_PORT = AUTHENTICATION_PORT + 1;

static const int MOVEMENT_DELAY_MILLIS = 400;

#endif // CONFIG
