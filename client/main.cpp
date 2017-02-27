#include "ui/window.h"

#include <QApplication>
#include <QtDebug>

int main(int argc, char *argv[]) {
    qSetMessagePattern("%{type} - %{file}(%{line}) %{function}(): %{message}");

    QApplication a(argc, argv);
    Window w;
    w.show();

    return a.exec();
}
