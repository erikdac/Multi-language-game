#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include "gamestate.h"
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "playerwidget.h"
#include "game/movementcontroller.h"
#include "ui/targetwidget.h"

#include <QWidget>

namespace Ui {
    class OnlineWidget;
}

class OnlineWidget : public GameState
{
    Q_OBJECT

    Ui::OnlineWidget * ui;

    MovementController * _movementController;

public:
    OnlineWidget(QWidget *);
    ~OnlineWidget();

    void resume() override;
    void pause() override;

    void switch_target(Player * player);

private:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void openMenu();
    void logout();

    TargetWidget * target_widget() const;

public slots:
    void input(std::string);
};

#endif // ONLINEWIDGET_H
