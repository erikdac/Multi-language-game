#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/entities/actor.h"
#include "game/entities/player.h"
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

signals:
    void activate(bool);
};

#endif // TARGETWIDGET_H
