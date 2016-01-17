#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game/player.h"
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

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void intitializeGL();
    void paintGL();
    void resizeGL();

private:
    Ui::GameWidget *ui;

    ScreenRefresher * _screenRefresher;
    std::unordered_map<char, KeyboardController*> _keyMap;

    void setScreenRefresher();
    void setKeyboardController(char);
    void openMenu();

public slots:
    void input(std::string);
};
#endif
