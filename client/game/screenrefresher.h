#ifndef SCREENREFRESHER_H
#define SCREENREFRESHER_H

#include <QThread>

class ScreenRefresher : public QThread {

    Q_OBJECT

    bool _isRunning;

public:
    ScreenRefresher();
    ~ScreenRefresher();
    void stop();
    void run();

signals:
    void repaint();
};


#endif // SCREENREFRESHER_H
