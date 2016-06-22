#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include "gamestate.h"

#include <QStackedWidget>
#include <vector>

class StackedWidget : public QStackedWidget {

    Q_OBJECT

    unsigned int _index = 0;
    std::vector<GameState *> _gameStates;

protected:
    StackedWidget(QWidget *parent);
    void addState(GameState *gameState);
    void setIndex(unsigned int index);
};

#endif
