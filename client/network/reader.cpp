#include "ui/loginwidget.h"
#include "json11/json11.hpp"
#include "network/reader.h"
#include "connection.h"
#include "ui/mainwindow.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <mutex>
#include <QWidget>
#include <QThread>
#include <QObject>
#include <QMetaType>

using namespace json11;

const static std::size_t BUFFER_SIZE= 131070;

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
    fd_set readfds;                     // Set of socket descriptors for select
    struct timeval tv;
    char readBuffer[BUFFER_SIZE + 2];   // Read buffer
    int received = 0;

    while(_isReading) {
        FD_ZERO(&readfds);
        FD_SET(s0, &readfds);
        tv = {0, 100 * 1000};

        int res = select(s0 + 1, &readfds, NULL, NULL, &tv);

        if (res < 0) {
            socket_error();
            break;
        } else if (res > 0) {
            res = read(
                s0,
                readBuffer + received,
                BUFFER_SIZE - received
            );

            if (res <= 0) {
                socket_error();
                break;
            }
            received += res;
        }

        // Success reading
        if (readBuffer[received-1] == 0 && received > 0) {
            emit input(readBuffer);
            received = 0;
        }
    }
}
