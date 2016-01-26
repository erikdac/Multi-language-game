#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"

#include <QOpenGLWidget>
#include <unordered_map>

namespace Ui {
    class GameWidget;
}

class GameWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void stop_refreshing();

protected:

    void intitializeGL();
    void paintGL();
    void resizeGL();

private:
    Ui::GameWidget *ui;

    ScreenRefresher * _screenRefresher;

    void setScreenRefresher();
};
#endif
