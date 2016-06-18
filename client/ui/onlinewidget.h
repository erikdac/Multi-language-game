#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include "game/objects/player.h"
#include "game/keyboardcontroller.h"

#include <QWidget>
#include <unordered_map>

namespace Ui {
    class OnlineWidget;
}

class OnlineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineWidget(QWidget *parent = 0);
    ~OnlineWidget();

    void start();

    void switch_target(Player * player);

private:
    Ui::OnlineWidget *ui;

    std::unordered_map<char, KeyboardController*> _keyMap;

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void openMenu();
    void logout();

public slots:
    void input(std::string);
};

#endif // ONLINEWIDGET_H
