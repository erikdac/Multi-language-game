#include "ui/loginwidget.h"
#include "json11/json11.hpp"
#include "reader.h"
#include "connection.h"

#include <QThread>
#include <QObject>
#include <QMetaType>

using namespace json11;

Reader::Reader() {
    qRegisterMetaType<std::string>();
}

Reader::~Reader() {

}

void Reader::stopReading() {
    _keepReading = false;
    while (_isRunning) {
        usleep(100 * 1000);
    }
}

void Reader::socket_error() {
    std::cerr << "Socket error" << std::endl;
    _keepReading = false;
    const Json data = Json::object {{"Type", "Disconnect"}};
    emit input(data.dump());
}

void Reader::run() {
    _isRunning = true;
    _keepReading = true;
    std::string packet = connection::readPacket(2);
    while(_keepReading) {
        if (packet == "Error") {
            socket_error();
        }
        else if (packet.size() > 0) {
            emit input(packet);
        }
        packet = connection::readPacket(2);
    }
    _isRunning = false;
}
