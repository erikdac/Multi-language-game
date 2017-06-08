#include "connection.h"
#include "external/json11/json11.hpp"

#include <QtDebug>
#include <QTcpSocket>

using namespace json11;

static const QString IP = "192.168.1.137";
static const int PORT = 1337;

static const std::size_t READ_BUFFER_SIZE = 64 * 1024;

static QTcpSocket _socket;

bool connection::connectToServer() {
    _socket.connectToHost(IP, PORT);
    if(!_socket.waitForConnected(10000)) {
        qDebug() << "Could not connect to server: " << _socket.errorString();
        return false;
    } else {
        qDebug() << "Successfully connected to server!";
        return true;
    }
}

bool connection::output(const Json object) {
    const std::string data = object.dump() + '\n';
    int res = _socket.write(data.c_str(), data.size() + 1);

    if (res < 0) {
        disconnect();
        bool isOnline = connectToServer();
        if(isOnline) {
            res = _socket.write(data.c_str(), data.size() + 1);
        } else {
            qDebug() << "Could not reconnect to server: " << _socket.errorString();
        }
    }

    if (res > 0) {
        _socket.flush();
    }

    return res > 0;
}

// TODO: Should only read UNTIL certain character...
std::string connection::readPacket(const int timeout_ms) {
    char readBuffer[READ_BUFFER_SIZE + 1];
    int received = 0;

    while (_socket.waitForReadyRead(timeout_ms)) {
        int res = _socket.readLine(readBuffer + received, READ_BUFFER_SIZE - received);
        received += res;

        if (res <= 0) {
            return "Network read error: " + _socket.errorString().toStdString();
        }

        if (received > 0 && readBuffer[received-1] == '\n') {
            break;
        }
    }
    return std::string(readBuffer);
}

void connection::disconnect() {
    qDebug() << "Disconnected!";
    _socket.disconnectFromHost();;
}
