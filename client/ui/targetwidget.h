#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/objects/player.h"
#include "ui/playerwidget.h"

#include <QWidget>

class TargetWidget : public PlayerWidget {

    Q_OBJECT

    Player _target;

public:
    TargetWidget(QWidget *parent);

    const Player & target() const;
    void check_target(const std::vector<Player *> &);
    void select_target(const Player &, bool);
    void unselect_target();
    void update_target(const Player &);

    void stop_attack();

protected:
    virtual void paintEvent(QPaintEvent *) override;
};

#endif // TARGETWIDGET_H
