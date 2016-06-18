#ifndef READER_H
#define READER_H

#include <iostream>
#include <QThread>

#include "json11/json11.hpp"

using namespace json11;

Q_DECLARE_METATYPE(std::string)

class Reader : public QThread
{
    Q_OBJECT

public:
    explicit Reader();
    ~Reader();
    void stopReading();
    void run();
    void socket_error();

private:
    bool _keepReading;
    bool _isRunning;

signals:
    void input(std::string);
};

#endif
