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

    bool _keepReading;
    bool _isRunning;

public:
    explicit Reader(QObject *);
    ~Reader();
    void stopReading();
    void run();

private:
    void socket_error();

signals:
    void input(std::string);
};

#endif
