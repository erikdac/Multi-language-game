#ifndef WINDOW_H
#define WINDOW_H

#include "gameengine/enginewidget.h"

namespace Ui {
    class Window;
}

class Window : public EngineWidget {

    Q_OBJECT

public:
    Window(QWidget * parent = 0);
    ~Window();

    void setLoginUi();
    void setLoadingUi();
    void setGameUi();

private:
    Ui::Window *ui;
};

#endif
