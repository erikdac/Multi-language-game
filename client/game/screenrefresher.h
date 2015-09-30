#ifndef SCREENREFRESHER_H
#define SCREENREFRESHER_H

#include <QThread>

class ScreenRefresher : public QThread {

    Q_OBJECT

public:
    ScreenRefresher();
    ~ScreenRefresher();
    void stop();
    void run();

private:
    bool _isRunning;


signals:
    void animate();
};


#endif // SCREENREFRESHER_H
