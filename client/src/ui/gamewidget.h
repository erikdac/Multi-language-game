#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "gameengine/gamestate.h"
#include "playerwidget.h"
#include "targetwidget.h"
#include "screenwidget.h"
#include "game/movementcontroller.h"
#include "game/eventhandler.h"
#include "json11/json11.hpp"

#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
    class GameWidget;
}

class GameWidget : public GameState {

    Q_OBJECT

    Ui::GameWidget * ui;

    ScreenWidget * _screenWidget;

    bool _isRunning;

    EventHandler<QMouseEvent *> _mouseHandler;
    EventHandler<std::pair<QKeyEvent, bool>> _keyboardHandler;

    MovementController _movementController;

public:
    GameWidget(QWidget *);
    ~GameWidget();

    void resume() override;
    void pause() override;
    void tick(float) override;

private:
    void processMouse();
    void processKeyboard();
    void processNetwork();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void networkReader();

    void openMenu();
    void logout();

    PlayerWidget * player_widget() const;
    TargetWidget * target_widget() const;
};

#endif // GAMEWIDGET_H
