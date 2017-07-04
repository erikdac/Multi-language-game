#include "test_connection.h"
#include "network/connection.h"
#include "json11/json11.hpp"

#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

using namespace json11;

static const int PORT = 1234;

void startServer() {
    std::condition_variable cv;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    auto func = [&]() -> void {
        QTcpServer server;
        server.listen(QHostAddress::LocalHost, PORT);
        cv.notify_all();
        if(server.waitForNewConnection(10000)) {
            QTcpSocket * socket = server.nextPendingConnection();
            int answer = 0;
            while (socket->waitForReadyRead(1000)) {
                socket->readLine();
                const Json response = Json::object {
                    {"Type", "Test"},
                    {"Value", answer++}
                };
                socket->write(response.dump().c_str());
                socket->write("\n");
                socket->flush();
            }
            socket->deleteLater();
        }
    };
    std::thread(func).detach();
    cv.wait(lock);
}

int getValue(const std::string & data) {
    return Json(data)["Value"].int_value();
}

void ConnectionTest::testAutenticate() {
    startServer();
    std::string ans1 = connection::authenticate("localhost", PORT, Json("{}"));
    QTRY_COMPARE(getValue(ans1), 0);
    std::string ans2 = connection::authenticate("localhost", PORT, Json("{}"));
    QTRY_COMPARE(getValue(ans2), 0);
}
