#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "game/player.h"
#include "game/keyboardcontroller.h"

#include <QWidget>
#include <unordered_map>

namespace Ui {
    class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);

private:
    Player * _player;
    std::unordered_map<char, std::string> _keyMap;

    Ui::GameWidget *ui;

    void openMenu();

public slots:
    void input(std::string);
    void animate();
};
#endif
