#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/objects/player.h"
#include "ui/playerwidget.h"

#include <QWidget>

class TargetWidget : public PlayerWidget {

    Q_OBJECT

public:
    TargetWidget(QWidget *parent = 0);

    Player * target() const;
    void select_target(Player * player);
    void unselect_target();
};

#endif // TARGETWIDGET_H
