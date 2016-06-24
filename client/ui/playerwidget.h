#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "game/objects/self.h"

#include <QWidget>

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
    void paint(QPaintEvent *, const Player & p);
};

#endif // PLAYERWIDGET_H
