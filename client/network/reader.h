#ifndef READER_H
#define READER_H

#include <iostream>
#include <QThread>

Q_DECLARE_METATYPE(std::string)

class Reader : public QThread
{
    Q_OBJECT

public:
    explicit Reader();
    ~Reader();
    void stopReading();
    void run();

private:
    bool _isReading;

signals:
    void input(std::string);
};

#endif
