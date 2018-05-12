#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "game/entities/self.h"
#include "game/entities/actor.h"

#include <QWidget>
#include <memory>

namespace Ui {
    class PlayerWidget;
}

class PlayerWidget : public QWidget {

    Q_OBJECT

    const Self * _player;

public:
    PlayerWidget(QWidget * parent);
    virtual ~PlayerWidget();

    void setPlayer(const Self *);

protected:
    Ui::PlayerWidget *ui;

    virtual void paintEvent(QPaintEvent *);
    void paint(QPaintEvent *, const Actor *);
};

#endif // PLAYERWIDGET_H
