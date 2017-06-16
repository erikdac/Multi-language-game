#include "connection.h"
#include "external/json11/json11.hpp"

#include <QtDebug>
#include <QTcpSocket>
#include <thread>

using namespace json11;

static const QString IP = "192.168.1.91";
static const int PORT = 1337;

static const std::size_t READ_BUFFER_SIZE = 64 * 1024;

QTcpSocket _socket;

EventHandler<json11::Json> connection::_inputHandler;
bool _isReading = false;

void connectToServer() {
    if (_socket.isValid()) {
        qWarning() << "Trying to set up a connection with an already existing one. ";
        connection::disconnect();
    }
    _socket.connectToHost(IP, PORT);
    if(!_socket.waitForConnected(10000)) {
        qDebug() << "Could not connect to server: " << _socket.errorString();
    } else {
        qDebug() << "Successfully connected to server!";
    }
}

bool connection::output(const Json object) {
    const std::string data = object.dump() + '\n';
    int res = _socket.write(data.c_str(), data.size());

    if (res < 0) {
        connectToServer();
        if(_socket.isWritable()) {
            res = _socket.write(data.c_str(), data.size());
        } else {
            qDebug() << "Could not reconnect to server: " << _socket.errorString();
        }
    }

    if (res > 0) {
        _socket.flush();
    }

    return res > 0;
}

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

void connection::startReading() {
    _isReading = true;
    auto func = [&]() -> void {
        qDebug() << "Starting to read from server!";
        while (_isReading) {
            std::string packet = connection::readPacket(10);
            if (!packet.empty()) {
                std::string error;
                Json data = Json::parse(packet, error);
                if (error.empty()) {
                    _inputHandler.addEvent(data);
                } else if (_isReading) {
                    qWarning("Incorrect JSON format recieved!");
                    data = Json::object {
                        {"Type", "Disconnect"},
                    };
                    _inputHandler.addEvent(data);
                    return;
                }
            }
        }
    };
    std::thread(func).detach();
}

void connection::disconnect() {
    qDebug() << "Disconnected!";
    _isReading = false;
    _socket.disconnectFromHost();
}
