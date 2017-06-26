#include "connection.h"
#include "json11/json11.hpp"
#include "game/eventhandler.h"

#include <QtDebug>
#include <QTcpSocket>
#include <thread>
#include <iostream>

using namespace json11;

static const QString IP = "192.168.1.91";
static const int PORT = 1337;

EventHandler<json11::Json> _inputHandler;
EventHandler<json11::Json> _outputHandler;

bool _isRunning = false;

void connection::write(const Json & data) {
    _outputHandler.addEvent(data);
}

std::vector<Json> connection::read() {
    return _inputHandler.events();
}

void connection::disconnect() {
    qDebug() << "Disconnected!";
    _isRunning = false;
}

void connectToServer(QTcpSocket & socket, const QString & ip, const int port) {
    qDebug() << "Attempting to connect to: " << ip << ":" << port;
    socket.connectToHost(ip, port);
    if(socket.waitForConnected(5000)) {
        qDebug() << "Successfully connected to server!";
    } else {
        qDebug() << "Could not connect to server: " << socket.errorString();
    }
}

void writing(QTcpSocket & socket) {
    for (const Json & object : _outputHandler.events()) {
        qDebug() << object.dump().c_str();
        const std::string data = object.dump() + '\n';
        int res = socket.write(data.c_str(), data.size());
        if (res < 0) {
            qDebug() << "Was not able to send packet to server: " << socket.errorString();
        } else {
            socket.flush();
        }
    }
}

std::string readPacket(QTcpSocket & socket, const int timeout_ms) {
    const std::size_t read_buffer_size = 128 * 1024;
    char readBuffer[read_buffer_size + 1];
    int received = 0;

    int additional_timeout = 0;
    while (socket.waitForReadyRead(timeout_ms + additional_timeout)) {
        int res = socket.readLine(readBuffer + received, read_buffer_size - received);
        received += res;

        if (res <= 0) {
            return "Network read error: " + socket.errorString().toStdString();
        } else if (received > 0 && readBuffer[received-1] == '\n') {
            return std::string(readBuffer);
        } else {
            additional_timeout = 4 * timeout_ms;
        }
    }
    return "";
}

void reading(QTcpSocket & socket) {
    std::string packet = readPacket(socket, 10);
    if (!packet.empty()) {
        std::string error;
        Json data = Json::parse(packet, error);
        if (error.empty()) {
            _inputHandler.addEvent(data);
        } else if (_isRunning) {
            qWarning() << "Incorrect JSON format recieved: " << packet.c_str();
            data = Json::object {
                {"Type", "Disconnect"},
            };
            _inputHandler.addEvent(data);
        }
    }
}

void connection::run(const std::string & token) {
    _isRunning = true;
    _outputHandler.events(); // Clear it
    auto func = [&](const std::string token) -> void {
        QTcpSocket socket;
        connectToServer(socket, IP, PORT + 1);
        if (socket.isValid()) {
            const Json packet = Json::object {
                {"Type", "Token"},
                {"Value", token}
            };
            const std::string data = packet.dump() + '\n';
            int res = socket.write(data.c_str(), data.size());
            if (res < 0) {
                qCritical() << "Could not write authentication token to server.";
                disconnect();
            } else {
                qDebug() << "Starting to communicate with the game server!";
                while (_isRunning) {
                    reading(socket);
                    writing(socket);
                }
                socket.disconnectFromHost();
            }
        }
    };
    std::thread(func, token).detach();
}

/**
 * @brief connection::authenticate
 * @param data
 * @return The authentication token
 */
std::string connection::authenticate(const Json & object) {
    QTcpSocket socket;
    connectToServer(socket, IP, PORT);
    if (socket.isValid()) {
        const std::string data = object.dump() + '\n';
        int res = socket.write(data.c_str(), data.size());
        if (res > 0) {
            qDebug() << "Authentication data sent to server.";
            return readPacket(socket, 5000);
        } else {
            qWarning() << "Could not write any data to the server!";
        }
    }
    return "";
}
