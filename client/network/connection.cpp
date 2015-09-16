#include "connection.h"
#include "loginwidget.h"
#include "json11/json11.hpp"
#include "network_reader.h"

#include <iostream>
#include <string.h>
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

Network_Reader * network_reader;

int s0; // Socket.

void setNetworkReader(Network_Reader *new_reader) {
    network_reader = new_reader;
}

void setActiveWidget(LoginWidget * object) {
    QObject::connect(
            network_reader,
            SIGNAL(input(std::string)),
            object,
            SLOT(input(std::string))
            );
}

static std::mutex output_mutex;

void output(const Json object) {

    std::string temp = object.dump();
    char data[temp.size()+1];
    strcpy(data, temp.c_str());

    output_mutex.lock();
    int res = write(s0, data, strlen(data) + 1);
    output_mutex.unlock();

    if (res < 0) {
        if (errno != EAGAIN)
            perror("Write error");
        else
            perror("Incompleted send");

    } else if (res == 0) {
        std::cout << "Connection closed" << std::endl;
    }
}

void disconnect() {
    network_reader->stopReading();
    shutdown(s0, 2);
    close(s0);
}

void sigHandler(int sigID) {
    std::cout << "The SIGPIPE signal (connection is broken)!" << std::endl;
    network_reader->stopReading();
    exit(1);
}

void connectToServer() {

    // Set signal handler for the "SIGPIPE" signal
    // (used to intercept the signal about broken connection).
    if (signal(SIGPIPE, &sigHandler) == SIG_ERR) {
        perror("Cannot install a signal handler");
        exit(1);
    }

    // Create a socket
    s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        perror("Cannot create a socket"); exit(1);
    }

    // Fill in the address of server
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    const char* peerHost = "localhost";

    // Resolve the server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL) {
        perror("Cannot define host address"); exit(1);
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
        perror("Cannot connect"); exit(1);
    }

    // Define socket as non-blocking
    res = fcntl(s0, F_SETFL, O_NONBLOCK);
    if (res < 0) {
        perror("Cannot set a socket as non-blocking");
        exit(1);
    }

    network_reader->start();
}
