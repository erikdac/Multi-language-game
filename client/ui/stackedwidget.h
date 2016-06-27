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
    StackedWidget(QWidget *);
    void addState(GameState *);
    GameState * currentState();

protected slots:
    void setIndex(unsigned int);

};

#endif
