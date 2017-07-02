#include "game/movementcontroller.h"
#include "game/entities/self.h"
#include "config.h"

#include <QtTest>
#include <QKeyEvent>
#include <thread>
#include <chrono>
#include <cmath>

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

Self emptySelf() {
    return Self(Player("", 0, 0, 0, 0, 0));
}

void MovementControllerTest::testSimpleMovement() {
    MovementController mc(0);
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.execute(&self);
    mc.released(Qt::Key_S);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_D);
    mc.execute(&self);
    mc.released(Qt::Key_D);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_W);
    mc.execute(&self);
    mc.released(Qt::Key_W);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_A);
    mc.execute(&self);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

void MovementControllerTest::testDirectionalMovement() {
    MovementController mc(0);
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.pushed(Qt::Key_D);
    mc.execute(&self);
    mc.released(Qt::Key_S);
    mc.released(Qt::Key_D);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_W);
    mc.pushed(Qt::Key_D);
    mc.execute(&self);
    mc.released(Qt::Key_W);
    mc.released(Qt::Key_D);
    QTRY_COMPARE(self.x(), 2);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.pushed(Qt::Key_A);
    mc.execute(&self);
    mc.released(Qt::Key_S);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_W);
    mc.pushed(Qt::Key_A);
    mc.execute(&self);
    mc.released(Qt::Key_W);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

void MovementControllerTest::testContradictoryMovement() {
    MovementController mc(0);
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_W);
    mc.pushed(Qt::Key_S);
    mc.execute(&self);
    mc.released(Qt::Key_S);
    mc.released(Qt::Key_W);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_S);
    mc.pushed(Qt::Key_W);
    mc.execute(&self);
    mc.released(Qt::Key_W);
    mc.released(Qt::Key_S);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_A);
    mc.pushed(Qt::Key_D);
    mc.execute(&self);
    mc.released(Qt::Key_D);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_D);
    mc.pushed(Qt::Key_A);
    mc.execute(&self);
    mc.released(Qt::Key_A);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

void MovementControllerTest::testClear() {
    MovementController mc(0);
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_W);
    mc.pushed(Qt::Key_A);
    mc.pushed(Qt::Key_S);
    mc.pushed(Qt::Key_D);
    mc.clear();
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

void MovementControllerTest::testHoldingKeys() {
    MovementController mc(0);
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 1);
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 2);
    mc.pushed(Qt::Key_D);
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 3);
    mc.execute(&self);
    mc.released(Qt::Key_S);
    QTRY_COMPARE(self.x(), 2);
    QTRY_COMPARE(self.y(), 4);
    mc.pushed(Qt::Key_W);
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 3);
    QTRY_COMPARE(self.y(), 3);
}

void MovementControllerTest::testMultiplyKeyPush() {
    MovementController mc(0);
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.released(Qt::Key_S);
    mc.pushed(Qt::Key_S);
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 1);
    mc.execute(&self);
    mc.released(Qt::Key_S);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 2);
    mc.pushed(Qt::Key_W);
    mc.execute(&self);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 1);
    mc.execute(&self);
    mc.released(Qt::Key_W);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

QTEST_MAIN(MovementControllerTest)
#include "test_movementcontroller.moc"
