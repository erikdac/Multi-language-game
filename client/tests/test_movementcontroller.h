#ifndef TEST_MOVEMENTCONTROLLER_H
#define TEST_MOVEMENTCONTROLLER_H

#include <QObject>

class MovementControllerTest : public QObject {

    Q_OBJECT

public:
    MovementControllerTest() {}

private Q_SLOTS:
    void testSimpleMovement();
    void testDirectionalMovement();
    void testContradictoryMovement();
    void testClear();
    void testHoldingKeys();
    void testMultiplyKeyPush();
};

#endif // TEST_MOVEMENTCONTROLLER_H
