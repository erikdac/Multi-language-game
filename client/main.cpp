#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <string.h>
#include "connection.hpp"
#include "json11/json11.hpp"

int main(int argc, char *argv[]) {

    connectToServer();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
