#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include "stackedwidget.h"

#include <QWidget>
#include <QObject>
#include <string>

class EngineWidget : public StackedWidget {

    Q_OBJECT

    int _max_update_rate;
    bool _isRunning;
    std::string _nextState;

protected:
    EngineWidget(const int max_update_rate, QWidget *);
    virtual ~EngineWidget();

    void run();

    void prepareState(const std::string);

private:
    void gameLoop();

};

#endif // ENGINEWIDGET_H
