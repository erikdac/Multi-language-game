#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include "handlers/mousehandler.h"

#include <QOpenGLWidget>
#include <QMouseEvent>

namespace Ui {
    class ScreenWidget;
}

class ScreenWidget : public QOpenGLWidget
{
    Q_OBJECT

    MouseHandler * _mouseHandler;

public:
    ScreenWidget(QWidget *);
    ~ScreenWidget();

    void setMouseHandler(MouseHandler *);

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
