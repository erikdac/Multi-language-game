#include "game/movementcontroller.h"

#include <QtTest>

class MovementControllerTest : public QObject {

    Q_OBJECT

public:
    MovementControllerTest() {}

private Q_SLOTS:
    void testCase1();
};

void MovementControllerTest::testCase1() {
    MovementController mc;
}

QTEST_MAIN(MovementControllerTest)
#include "test_movementcontroller.moc"
