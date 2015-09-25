#ifndef READER_H
#define READER_H

#include <iostream>
#include <QThread>

Q_DECLARE_METATYPE(std::string)

extern int s0;

class Reader : public QThread
{
    Q_OBJECT

public:
    explicit Reader();
    ~Reader();
    void stopReading();
    void run();
    void handleInput(const char readBuffer[]);

private:
    int _isReading;

signals:
    void input(std::string);
};

#endif
