#include "ui/window.h"

#include <QApplication>


#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window w;
    w.show();

    return a.exec();
}
