#ifndef Network_Reader_H
#define Network_Reader_H

#include <iostream>
#include <QThread>

Q_DECLARE_METATYPE(std::string)

extern int s0;

class Network_Reader : public QThread
{
    Q_OBJECT

public:
    explicit Network_Reader(QWidget *parent);
    ~Network_Reader();
    void stopReading();
    void run();
    void handleInput(const char readBuffer[]);

private:
    int _isReading;

signals:
    void input(std::string);
};

#endif // Network_Reader_H
