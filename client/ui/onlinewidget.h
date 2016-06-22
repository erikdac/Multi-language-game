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

    Ui::OnlineWidget *ui;

    MovementController * _movementController;

public:
    void init(QWidget *) override;
    void clear() override;

    void resume() override;
    void pause() override;

    static OnlineWidget * instance() {
        static OnlineWidget * _instance = new OnlineWidget();
        return _instance;
    }

    TargetWidget * target_widget() const;
    void switch_target(Player * player);

protected:
    OnlineWidget();
    ~OnlineWidget();

private:

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void openMenu();
    void logout();

public slots:
    void input(std::string);
};

#endif // ONLINEWIDGET_H
