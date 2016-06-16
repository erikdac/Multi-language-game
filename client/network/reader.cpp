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
    _isReading = false;
}

void Reader::socket_error() {
    std::cerr << "Socket error" << std::endl;
    stopReading();
    const Json data = Json::object {{"Type", "Disconnect"}};
    emit input(data.dump());
}

void Reader::run() {
    _isReading = true;
    while(_isReading) {
        std::string packet = connection::readPacket();
        if (packet.size() > 0) {
            emit input(packet);
        }
    }
}
