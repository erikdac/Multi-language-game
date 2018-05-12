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
#include <random>

using namespace json11;

static const int PORT = 1234;

std::string randomString(const int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> random(0, (sizeof(alphanum) - 1));

    std::string str = "";
    for (int i = 0; i < length; ++i) {
        str += alphanum[random(gen)];
    }
    return str;
}

void communicateServer(QTcpSocket * const socket, const int latency_ms) {
    int answer = 0;
    qDebug() << "BEFORE";
    while (socket->waitForReadyRead(100)) {
        socket->readLine();
        std::this_thread::sleep_for(std::chrono::milliseconds(latency_ms));
//        qDebug() << QString(randomString(16).c_str());
        const Json response = Json::object {
            {"Type", "Test"},
            {"Value", answer++},
            {"Message", randomString(128 * 1024)}
        };
        socket->write(response.dump().c_str());
        socket->write("\n");
        socket->flush();
    }
    qDebug() << "AFTER";
    socket->deleteLater();
}

void startServer(const int latency_ms) {
    std::condition_variable cv;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    auto func = [&]() -> void {
        QTcpServer server;
        server.listen(QHostAddress::LocalHost, PORT);
        cv.notify_all();
        while (server.waitForNewConnection(2000)) {
            QTcpSocket * socket = server.nextPendingConnection();
            communicateServer(socket, latency_ms);
        }
    };
    std::thread(func).detach();
    cv.wait(lock);
}

// ****************************************************************************

int getValue(const std::string & data) {
    return Json(data)["Value"].int_value();
}

void ConnectionTest::testAutenticate() {
    startServer(0);
    std::string ans1 = connection::authenticate("localhost", PORT, Json("{}"));
    QTRY_COMPARE(getValue(ans1), 0);
    connection::disconnect();
    std::string ans2 = connection::authenticate("localhost", PORT, Json("{}"));
    QTRY_COMPARE(getValue(ans2), 0);
}

void ConnectionTest::testHighLatency() {
    startServer(0);
    std::string ans1 = connection::authenticate("localhost", PORT, Json("{}"));
    QTRY_COMPARE(getValue(ans1), 0);
}
