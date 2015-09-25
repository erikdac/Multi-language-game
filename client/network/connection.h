#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QThread>
#include "loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

extern int s0;

void setActiveWidget(QWidget *);
bool online();
bool connectToServer();
void readInput();
void output(const Json);
void sigHandler(int);
void disconnect();

#endif
