#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game/screenrefresher.h"

#include <QOpenGLWidget>

namespace Ui {
    class GameWidget;
}

class GameWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void start_refreshing();
    void stop_refreshing();

protected:
    void intitializeGL();
    void paintGL();
    void resizeGL();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::GameWidget *ui;

    ScreenRefresher * _screenRefresher;

};
#endif
