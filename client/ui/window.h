#ifndef WINDOW_H
#define WINDOW_H

#include "stackedwidget.h"

namespace Ui {
    class Window;
}

class Window : public StackedWidget {

    Q_OBJECT

    bool _isRunning;
    int _nextIndex = -1;

public:
    explicit Window(QWidget * parent = 0);
    ~Window();

    void gameLoop();

    void setLoginUi();
    void setLoadingUi();
    void setGameUi();

private:
    Ui::Window *ui;
};

#endif
