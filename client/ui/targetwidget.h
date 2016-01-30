#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include "game/objects/player.h"
#include "ui/playerwidget.h"

#include <QWidget>
#include <QThread>
#include <mutex>

class TargetWidget : public PlayerWidget {

    Q_OBJECT

    bool _combat;

public:
    TargetWidget(QWidget *parent = 0);
    ~TargetWidget();

    Player * target() const;
    void select_target(Player *, bool);
    void unselect_target();

private:
    class Attack : public QThread {

        TargetWidget * const _outer;
        const unsigned int ATTACK_DELAY_MS = 2000;

    public:
        Attack(TargetWidget * const);

        void run();

        std::mutex attack_mutex;

    private:
        void hit();
        void safe_sleep(const unsigned int);

    } _attack;
};

#endif // TARGETWIDGET_H
