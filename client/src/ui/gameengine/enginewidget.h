#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include "stackedwidget.h"

#include <QWidget>
#include <QObject>
#include <string>
#include <chrono>

class EngineWidget : public StackedWidget {

    Q_OBJECT

    int _max_update_rate;
    bool _isRunning;
    std::string _nextState;

protected:
    EngineWidget(const int, QWidget *);
    virtual ~EngineWidget();

    void run();

    void prepareState(const std::string);

private:
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

    void gameLoop();
    int timeSince(TimePoint) const;

};

#endif // ENGINEWIDGET_H
