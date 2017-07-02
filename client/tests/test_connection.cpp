/*
#include "network/connection.h"

#include <QtTest>
#include <QKeyEvent>
#include <QTcpServer>

class ConnectionTest : public QObject {

    Q_OBJECT

public:
    MovementControllerTest() {}

private Q_SLOTS:
    void testAutenticate();
};

class Server : public QObject {

    Q_OBJECT

    QTcpServer _server;
    int _answer = 0;

public:
    Server(QObject *parent = 0) : _server(QTcpServer(this)) {
        if(!server->listen(QHostAddress::Any, 0)) {
            qDebug() << "Server could not start"; }
        }
    };

public slots:
    void newConnection() {
        QTcpSocket * socket = _server.nextPendingConnection();
        socket->write("Hello client\r\n");
        socket->flush();
        socket->waitForBytesWritten(3000); socket->close();
    }

};

void ConnectionTest::testAutenticate() {

}

QTEST_MAIN(ConnectionTest)
#include "test_connection.moc"
*/
