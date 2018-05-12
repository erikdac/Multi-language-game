#ifndef CONFIG
#define CONFIG

#include <QString>

static const int MAX_UPDATE_RATE = 100;
static constexpr int MAX_FPS = 60 - 1;

static const QString IP = "192.168.1.240";
static const int AUTHENTICATION_PORT = 1337;
static const int GAME_PORT = AUTHENTICATION_PORT + 1;

static const int MOVEMENT_DELAY_MILLIS = 400;

#endif // CONFIG
