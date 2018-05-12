#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include "gameengine/glwidget.h"
#include "game/eventhandler.h"

#include <QMouseEvent>

namespace Ui {
    class ScreenWidget;
}

class ScreenWidget : public GLWidget {

    Q_OBJECT

    EventHandler<QMouseEvent *> * _mouseHandler;

public:
    ScreenWidget(QWidget *);
    virtual ~ScreenWidget();

    void setMouseHandler(EventHandler<QMouseEvent *> *);

protected:
    void paintGL() override;
    void mousePressEvent(QMouseEvent *);

private:
    Ui::ScreenWidget *ui;
};

#endif
