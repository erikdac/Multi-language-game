#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "game/objects/self.h"

#include <QWidget>

namespace Ui {
    class PlayerWidget;
}

class PlayerWidget : public QWidget {

    Q_OBJECT

public:
    explicit PlayerWidget(QWidget * parent = 0);
    virtual ~PlayerWidget();

    void setPlayer(Self *);

protected:
    Ui::PlayerWidget *ui;
    Self * _player;

    virtual void paintEvent(QPaintEvent *);
    void paint(QPaintEvent *, Player & p);
};

#endif // PLAYERWIDGET_H
