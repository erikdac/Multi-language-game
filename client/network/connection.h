#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QThread>
#include "network/reader.h"
#include "loginwidget.h"

#include "json11/json11.hpp"

using namespace json11;

extern int s0;

void setNetworkReader(Reader * const);
void setActiveWidget(QWidget *);
bool online();
void connectToServer();
void readInput();
void output(const Json);
void sigHandler(int);
void disconnect();

#endif
