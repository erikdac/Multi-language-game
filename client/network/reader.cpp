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

void Reader::socket_error(const int res) {
    if (res < 0 && errno != EAGAIN) {
        perror("Read error");
    }
    _isReading = false;
    const Json data = Json::object {{"Type", "Disconnect"}};
    emit input(data.dump());
}

void Reader::run() {

    fd_set readfds;                     // Set of socket descriptors for select
    struct timeval tv;
    char readBuffer[BUFFER_SIZE + 2];   // Read buffer
    _isReading = true;

    while(_isReading) {
        int received = 0;
        FD_ZERO(&readfds);    // Erase the set of socket descriptors
        FD_SET(s0, &readfds); // Add the socket "s0" to the set

        tv = {1, 0};        // Timeout value for the reading to finish.

        //=== "select" is the key point of the program! ============
        int res = select(
            s0 + 1,   // Max. number of socket in all sets + 1
            &readfds, // Set of socket descriptors for reading
            NULL,     // Set of sockets for writing -- not used
            NULL,     // Set of sockets with exceptions -- not used
            &tv       // Timeout value
        );
        //=========================================================

        if (res < 0) {
            perror("Select error");
            _isReading = false;
            const Json data = Json::object {{"Type", "Disconnect"}};
            emit input(data.dump());
        } else if (res > 0) {
            res = read(
                s0,
                readBuffer + received,
                BUFFER_SIZE - received
            );

            if (res > 0) {
                received += res;
                readBuffer[received] = '\0';
                emit input(readBuffer);
            }
            else {
                socket_error(res);
            }
        }
    }
}
