#include "connection.h"
#include "json11/json11.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <cassert>

using namespace json11;

static const std::string IP = "localhost";
static const int PORT = 1337;

static const std::size_t READ_BUFFER_SIZE= 128 * 1024;

int _s0; // Socket.

bool connection::connectToServer() {

    // Create a socket
    _s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (_s0 < 0) {
        return false;
    }

    // Fill in the address of server
    struct sockaddr_in peeraddr;
    std::memset(&peeraddr, 0, sizeof(peeraddr));

    // Resolve the server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(IP.c_str());
    if (host == NULL) {
        return false;
    }

    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(PORT);

    // Write resolved IP address of a server to the address structure
    std::memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to a remote server
    int res = connect(_s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
    if (res < 0) {
        return false;
    }

    // Define socket as non-blocking
    res = fcntl(_s0, F_SETFL, O_NONBLOCK);
    if (res < 0) {
        return false;
    }

    return true;
}

bool connection::output(const Json object) {
    const std::string data = object.dump();
    int res = write(_s0, data.c_str(), data.size() + 1);

    if (res < 0) {
        disconnect();
        bool isOnline = connectToServer();
        if(isOnline) {
            res = write(_s0, data.c_str(), data.size() + 1);
        } else {
            assert(res < 0);
        }
    }

    return res >= 0;
}

std::string connection::readPacket(const int timeout_ms) {
    fd_set readfds;
    char readBuffer[READ_BUFFER_SIZE + 1];
    int received = 0;

    for (int i = 0; i < 2; ++i) {
        FD_ZERO(&readfds);
        FD_SET(_s0, &readfds);
        struct timeval tv = {0, 500 * timeout_ms};

        int res = select(_s0 + 1, &readfds, NULL, NULL, &tv);

        if (res < 0) {
            return "Connection error";
        } else if (res > 0) {
            res = read(
                _s0,
                readBuffer + received,
                READ_BUFFER_SIZE - received
            );

            if (res <= 0) {
                return "network read error";
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

void connection::disconnect()  {
    close(_s0);
}
