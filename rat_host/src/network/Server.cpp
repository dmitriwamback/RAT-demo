//
// Created by Dmitri on 2026-10-27.
//

#include "Server.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>

Server::~Server() {
    Stop();
    if (listen_fd >= 0) {
        close(listen_fd);
        listen_fd = -1;
    }
}

void Server::Log(const std::string &msg) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << msg << std::endl;
}

bool Server::Start(const std::string& port) {

    addrinfo hints{}, *result = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (int rc = getaddrinfo(nullptr, port.c_str(), &hints, &result); rc != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(rc) << std::endl;
        return false;
    }

    listen_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_fd < 0) {
        perror("socket");
        freeaddrinfo(result);
        return false;
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    if (bind(listen_fd, result->ai_addr, result->ai_addrlen) < 0 || listen(listen_fd, SOMAXCONN) < 0) {
        perror("bind");
        freeaddrinfo(result);
        close(listen_fd);
        listen_fd = -1;
        return false;
    }
    freeaddrinfo(result);

    running = true;
    Log("Running on port " + port);

    return true;
}

void Server::Run() {
    while (running) {

        pollfd pfd{listen_fd, POLLIN, 0};
        if (poll(&pfd, 1, 200) < 0) {
            continue;
        }

        sockaddr_in caddr{};
        socklen_t addrlen = sizeof(caddr);
        int client_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&caddr), &addrlen);
        if (client_fd < 0) {
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &caddr.sin_addr, ip, sizeof(ip));

        std::string addr = std::string(ip) + ":" + std::to_string(ntohs(caddr.sin_port));

        {
            std::lock_guard<std::mutex> lock(clientMutex);
            client_fds.push_back(client_fd);
        }
        workers.emplace_back(&Server::HandleClient, this, client_fd, addr);
    }

    for (auto& worker : workers) {
        worker.join();
    }
    workers.clear();
}

void Server::Stop() {
    running = false;
    std::lock_guard<std::mutex> lock(clientMutex);
    for (int fd : client_fds) {
        shutdown(fd, SHUT_RDWR);
    }
}

void Server::HandleClient(int client_fd, std::string addr) {
    clientCount++;
    Log("[+] " + addr + " connected");

    char buffer[1024];
    while (true) {
        ssize_t n = recv(client_fd, buffer, 1024, 0);
        if (n <= 0) {
            break;
        }

        std::string message(buffer, n);
        Log("[+] " + addr + " received: " + message);
    }
}

bool Server::Send(int client_fd, const std::string &message) {

    std::lock_guard<std::mutex> lock(clientMutex);
    if (std::find(client_fds.begin(), client_fds.end(), client_fd) == client_fds.end()) {
        return false;
    }

    size_t sent = 0;
    while (sent < message.length()) {
        ssize_t n = send(client_fd, message.data() + sent, message.size() - sent, 0);
        if (n <= 0) {
            return false;
        }
        sent += n;
    }

    return true;
}

void Server::Broadcast(const std::string &message) {
    for (int client_fd : ClientIds()) {
        Send(client_fd, message);
    }
}

std::vector<int> Server::ClientIds() {
    std::lock_guard<std::mutex> lock(clientMutex);
    return client_fds;
}
