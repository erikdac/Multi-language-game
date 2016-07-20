#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/objects/actor.h"
#include "game/objects/player.h"
#include "ui/playerwidget.h"

#include <QWidget>

class TargetWidget : public PlayerWidget {

    Q_OBJECT

    Actor * _target = 0;

public:
    TargetWidget(QWidget *parent);

    std::string target() const;
    void check_target(const std::vector<Actor *> &);
    void select_target(Actor *, bool);
    void unselect_target();
    void update_target(Actor *);

    void stop_attack();

protected:
    virtual void paintEvent(QPaintEvent *) override;
};

#endif // TARGETWIDGET_H
