#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/objects/player.h"
#include "ui/playerwidget.h"

#include <QWidget>
#include <QThread>
#include <mutex>

class TargetWidget : public PlayerWidget {

    Q_OBJECT

    std::mutex attack_mutex;

public:
    TargetWidget(QWidget *parent = 0);

    Player * target() const;
    void select_target(Player *, bool);
    void unselect_target();
    void update_target(Player *);

    void stop_attack();
};

#endif // TARGETWIDGET_H