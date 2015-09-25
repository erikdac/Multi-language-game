#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include "loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

extern int s0;

void setActiveWidget(QWidget *);
bool online();
bool connectToServer();
bool output(const Json);
void sigHandler(int);
void disconnect();

#endif
