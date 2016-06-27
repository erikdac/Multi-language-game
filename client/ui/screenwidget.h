#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include "game/eventhandler.h"

#include <QOpenGLWidget>
#include <QMouseEvent>

namespace Ui {
    class ScreenWidget;
}

class ScreenWidget : public QOpenGLWidget
{
    Q_OBJECT

    EventHandler<QMouseEvent *> * _mouseHandler;

public:
    ScreenWidget(QWidget *);
    ~ScreenWidget();

    void setMouseHandler(EventHandler<QMouseEvent *> *);

protected:
    void intitializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::ScreenWidget *ui;

signals:
    void repaint();
};

#endif
