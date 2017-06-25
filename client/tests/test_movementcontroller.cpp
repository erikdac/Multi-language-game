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
};

void waitForReady() {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(MOVEMENT_DELAY_MILLIS * std::sqrt(2))));
}

Self emptySelf() {
    return Self(Player("", 0, 0, 0, 0, 0));
}

void MovementControllerTest::testSimpleMovement() {
    MovementController mc;
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_S);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_D);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_D);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_W);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_W);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_A);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

void MovementControllerTest::testDirectionalMovement() {
    MovementController mc;
    Self self = emptySelf();
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.pushed(Qt::Key_D);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_S);
    mc.released(Qt::Key_D);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_W);
    mc.pushed(Qt::Key_D);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_W);
    mc.released(Qt::Key_D);
    QTRY_COMPARE(self.x(), 2);
    QTRY_COMPARE(self.y(), 0);
    mc.pushed(Qt::Key_S);
    mc.pushed(Qt::Key_A);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_S);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 1);
    QTRY_COMPARE(self.y(), 1);
    mc.pushed(Qt::Key_W);
    mc.pushed(Qt::Key_A);
    waitForReady();
    mc.execute(&self);
    mc.released(Qt::Key_W);
    mc.released(Qt::Key_A);
    QTRY_COMPARE(self.x(), 0);
    QTRY_COMPARE(self.y(), 0);
}

void MovementControllerTest::testContradictoryMovement() {

}

QTEST_MAIN(MovementControllerTest)
#include "test_movementcontroller.moc"
