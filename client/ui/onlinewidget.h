#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include "gamestate.h"
#include "game/objects/player.h"
#include "playerwidget.h"
#include "game/movementcontroller.h"
#include "ui/targetwidget.h"
#include "ui/screenwidget.h"
#include "handlers/mousehandler.h"
#include "handlers/keyboardhandler.h"

#include <QWidget>

namespace Ui {
    class OnlineWidget;
}

class OnlineWidget : public GameState {

    Q_OBJECT

    Ui::OnlineWidget * ui;

    ScreenWidget * _screenWidget;
    MouseHandler _mouseHandler;
    KeyboardHandler _keyboardHandler;
    MovementController * _movementController;

    bool _isRunning;
    bool _isFinished;

public:
    OnlineWidget(QWidget *);
    ~OnlineWidget();

    void resume() override;
    void pause() override;
    void process() override;

private:
    void processMouse();
    void processKeyboard();
    void processNetwork();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void openMenu();
    void logout();

    TargetWidget * target_widget() const;
};

#endif // ONLINEWIDGET_H
