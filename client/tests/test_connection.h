#ifndef TEST_CONNECTION_H
#define TEST_CONNECTION_H

#include <QObject>

class ConnectionTest : public QObject {

    Q_OBJECT

public:
    ConnectionTest() {}

private Q_SLOTS:
    void testAutenticate();
    void testHighLatency();
};

#endif // TEST_CONNECTION_H
