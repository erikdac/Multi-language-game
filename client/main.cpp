#include "ui/window.h"
#include "config.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window w(MAX_UPDATE_RATE);
    w.run();
    w.show();

    return a.exec();
}
