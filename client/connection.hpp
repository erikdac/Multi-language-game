#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QThread>
#include "reader.h"
#include "loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

extern int s0;

void setReader(Reader * reader);
void setActiveWidget(LoginWidget * object);
bool online();
void connectToServer();
void readInput();
void output(Json object);
void sigHandler(int sigID);
void disconnect();

#endif
