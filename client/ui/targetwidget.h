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
    void select_target(Actor *);
    void unselect_target();
    void update_target(Actor *);


protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    void stop_attack();
};

#endif // TARGETWIDGET_H
