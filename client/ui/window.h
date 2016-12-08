#ifndef WINDOW_H
#define WINDOW_H

#include "stackedwidget.h"

namespace Ui {
    class Window;
}

class Window : public StackedWidget {

    Q_OBJECT

    int _max_update_rate;
    bool _isRunning;
    int _nextIndex = -1;

public:
    Window(const int, QWidget * parent = 0);
    ~Window();

    void run();

    void gameLoop();

    void setLoginUi();
    void setLoadingUi();
    void setGameUi();

private:
    Ui::Window *ui;
};

#endif
