#include "connection.h"
#include "ui/loginwidget.h"
#include "json11/json11.hpp"
#include "reader.h"
#include "game/map.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <QWidget>
#include <QThread>

using namespace json11;

int s0; // Socket.
bool _online = false;

Reader * const reader = new Reader();

void connection::setActiveWidget(QWidget * object) {
    QObject::connect(
        reader,
        SIGNAL(input(std::string)),
        object,
        SLOT(input(std::string))
    );
}

static std::mutex output_mutex;

bool connection::output(const Json object) {
    if(!_online) {
        if(connection::connectToServer() == false) {
            return false;
        }
        else {
            _online = true;
        }
    }

    std::string temp = object.dump();
    char data[temp.size()+1];
    strcpy(data, temp.c_str());

    output_mutex.lock();
    int res = write(s0, data, strlen(data) + 1);
    output_mutex.unlock();

    if (res <= 0) {
        if (errno != EAGAIN) {
            perror("Write error");
        }
        else if(res == 0) {
            std::cerr << "Connection closed" << std::endl;
        }

        else {
            perror("Incompleted send");
        }
        return false;
    }
    else
        return true;
}

void connection::disconnect() {
    reader->stopReading();
    shutdown(s0, 2);
    close(s0);
    cleanMap();
    _online = false;
}

void sigHandler(int sigID) {
    std::cout << "The SIGPIPE signal (connection is broken)!" << std::endl;
    reader->stopReading();
}

bool connection::connectToServer() {

    // Set signal handler for the "SIGPIPE" signal
    // (used to intercept the signal about broken connection).
    if (signal(SIGPIPE, &sigHandler) == SIG_ERR) {
        perror("Cannot install a signal handler");
        return false;
    }

    // Create a socket
    s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        perror("Cannot create a socket");
        return false;
    }

    // Fill in the address of server
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    const char* peerHost = "localhost";

    // Resolve the server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL) {
        perror("Cannot define host address");
        return false;
    }
    peeraddr.sin_family = AF_INET;
    const short peerPort = 1337;

    peeraddr.sin_port = htons(peerPort);

    // Print a resolved address of server (the first IP of the host)
    printf(
        "Peer address = %d.%d.%d.%d, Port %d\n",
        host->h_addr_list[0][0] & 0xff,
        host->h_addr_list[0][1] & 0xff,
        host->h_addr_list[0][2] & 0xff,
        host->h_addr_list[0][3] & 0xff,
        (int) peerPort
    );

    // Write resolved IP address of a server to the address structure
    memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to a remote server
    int res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
    if (res < 0) {
        perror("Cannot connect");
        return false;
    }

    // Define socket as non-blocking
    res = fcntl(s0, F_SETFL, O_NONBLOCK);
    if (res < 0) {
        perror("Cannot set a socket as non-blocking");
        return false;
    }

    reader->start();
    return true;
}
