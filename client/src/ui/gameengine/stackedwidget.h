#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include "gamestate.h"

#include <QStackedWidget>
#include <string>
#include <map>

class StackedWidget : public QStackedWidget {

    Q_OBJECT

    int _index = 0;
    std::map<std::string, int> _gameStates;

protected:
    StackedWidget(QWidget *);
    virtual ~StackedWidget();
    void addState(std::string, GameState *);
    GameState * currentState();

    void changeToState(const std::string);

private:
    GameState * state(const int);

private slots:
    void changeUi(const int);

signals:
    void executeChange(const int);
    void done();
};

#endif
