#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include <QWidget>
#include "game/objects/player.h"
#include "game/keyboardcontroller.h"
#include "game/screenrefresher.h"

#include <QOpenGLWidget>
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

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    Ui::OnlineWidget *ui;

    std::unordered_map<char, KeyboardController*> _keyMap;

    void setKeyboardController(char);
    void openMenu();
    void logout();

public slots:
    void input(std::string);
};

#endif // ONLINEWIDGET_H
