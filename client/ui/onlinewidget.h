#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include "gamestate.h"
#include "game/objects/player.h"
#include "playerwidget.h"
#include "game/movementcontroller.h"
#include "ui/targetwidget.h"
#include "ui/screenwidget.h"
#include "game/eventhandler.h"

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <utility>

namespace Ui {
    class OnlineWidget;
}

class OnlineWidget : public GameState {

    Q_OBJECT

    Ui::OnlineWidget * ui;

    ScreenWidget * _screenWidget;

    EventHandler<QMouseEvent *> _mouseHandler;
    EventHandler<std::pair<QKeyEvent, bool>> _keyboardHandler;

    MovementController * _movementController;

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
