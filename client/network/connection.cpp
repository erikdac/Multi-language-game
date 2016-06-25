#include "connection.h"
#include "ui/loginwidget.h"
#include "json11/json11.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <mutex>

using namespace json11;

const static std::size_t READ_BUFFER_SIZE= 128 * 1024;

int s0; // Socket.
bool _online = false;

bool connectToServer() {

    // Create a socket
    s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        return false;
    }

    // Fill in the address of server
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    const char* peerHost = "localhost";

    // Resolve the server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL) {
        return false;
    }
    peeraddr.sin_family = AF_INET;
    const short peerPort = 1337;

    peeraddr.sin_port = htons(peerPort);

    // Write resolved IP address of a server to the address structure
    memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to a remote server
    int res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
    if (res < 0) {
        return false;
    }

    // Define socket as non-blocking
    res = fcntl(s0, F_SETFL, O_NONBLOCK);
    if (res < 0) {
        return false;
    }

    return true;
}

bool brokenConnection() {
    if(!_online) {
        if(connectToServer() == false) {
            return true;
        }
        else {
            _online = true;
        }
    }
    return false;
}

static std::mutex output_mutex;

bool connection::output(const Json object) {
    if(brokenConnection()) {
        return false;
    }

    const std::string data = object.dump();
    output_mutex.lock();
    int res = write(s0, data.c_str(), data.size() + 1);
    output_mutex.unlock();

    return res > 0;
}

std::string connection::readPacket(const unsigned int timeout_ms) {
    if(brokenConnection()) {
        return "";
    }

    fd_set readfds;
    char readBuffer[READ_BUFFER_SIZE + 1];
    int received = 0;

    for (unsigned int i = 0; i < timeout_ms; ++i) {
        FD_ZERO(&readfds);
        FD_SET(s0, &readfds);
        struct timeval tv = {0, 1000};

        int res = select(s0 + 1, &readfds, NULL, NULL, &tv);

        if (res < 0) {
            return "Error";
        } else if (res > 0) {
            res = read(
                s0,
                readBuffer + received,
                READ_BUFFER_SIZE - received
            );

            if (res <= 0) {
                return "Error";
            } else {
                received += res;
                i = 0;
            }
        }

        // Success reading
        if (readBuffer[received-1] == 0 && received > 0) {
            return std::string(readBuffer);
        }
    }
    return "";
}

void connection::disconnect() {
    if (_online) {
        _online = false;
        shutdown(s0, 2);
        close(s0);
    }
}
