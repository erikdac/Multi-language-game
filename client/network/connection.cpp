#include "connection.h"
#include "external/json11/json11.hpp"

#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cassert>

#include <QtDebug>
#include <QTcpSocket>

using namespace json11;

static const QString IP = "localhost";
static const int PORT = 1337;

static const std::size_t READ_BUFFER_SIZE = 64 * 1024;

QTcpSocket _socket;

bool connection::connectToServer() {
    _socket.connectToHost(IP, PORT);
    if(!_socket.waitForDisconnected(1000)) {
        qDebug() << "Could not connect to server: " << _socket.errorString();
        return false;
    } else {
        qDebug() << "Successfully connected to server!";
        return true;
    }
}

bool connection::output(const Json object) {
    const std::string data = object.dump();
    int res = _socket.write(data.c_str());

    if (res < 0) {
        disconnect();
        bool isOnline = connectToServer();
        if(isOnline) {
            res = _socket.write(data.c_str());
        } else {
            qDebug() << "Could not reconnect to server: " << _socket.errorString();
        }
    }

    return res >= 0;
}

// TODO: Should only read UNTIL certain character...
std::string connection::readPacket(const int timeout_ms) {
    char readBuffer[READ_BUFFER_SIZE + 1];
    int received = 0;

    while (_socket.waitForReadyRead(timeout_ms)) {
        int res = _socket.read(readBuffer + received, READ_BUFFER_SIZE - received);
        if (res <= 0) {
            return "Network read error: " + _socket.errorString().toStdString();
        }

        // Success reading
        if (received > 0 && readBuffer[received-1] == 0) {
            break;
        }
    }
    return std::string(readBuffer);
}

void connection::disconnect() {
    qDebug() << "Disconnected!";
    _socket.disconnectFromHost();;
}
