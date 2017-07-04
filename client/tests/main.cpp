#include "testrunner.h"
#include "test_connection.h"
#include "test_movementcontroller.h"

#include <QtTest>
#include <iostream>

int main() {
    TestRunner testRunner;
    testRunner.addTest(new ConnectionTest);
    testRunner.addTest(new MovementControllerTest);

    qDebug() << "Overall result: " << (testRunner.runTests()?"PASS":"FAIL");

    return 0;
}
