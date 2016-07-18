#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/objects/player.h"
#include "ui/playerwidget.h"

#include <QWidget>

class TargetWidget : public PlayerWidget {

    Q_OBJECT

    Player * _target = 0;

public:
    TargetWidget(QWidget *parent);

    std::string target() const;
    void check_target(const std::vector<Player *> &);
    void select_target(Player *, bool);
    void unselect_target();
    void update_target(Player *);

    void stop_attack();

protected:
    virtual void paintEvent(QPaintEvent *) override;
};

#endif // TARGETWIDGET_H
