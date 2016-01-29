#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "game/objects/player.h"

#include <QWidget>

namespace Ui {
    class PlayerWidget;
}

class PlayerWidget : public QWidget {

    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0, Player * player = 0);
    virtual ~PlayerWidget();

    virtual void update();

protected:
    Ui::PlayerWidget *ui;
    Player * _player;
};

#endif // PLAYERWIDGET_H
