#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QWidget>

class GameState : public QWidget {

public:
    virtual void init(QWidget *) = 0;
    virtual void clear() = 0;

    virtual void resume() = 0;
    virtual void pause() = 0;
};

#endif // GAMESTATE_H
