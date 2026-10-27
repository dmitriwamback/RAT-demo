//
// Created by Dmitri on 2026-10-27.
//

#ifndef RAT_HOST_SERVER_H
#define RAT_HOST_SERVER_H
#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Server {
public:
    Server() = default;
    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    bool Start(const std::string& port);
    void Run();
    void Stop();
    bool Send(int client_fd, const std::string& message);
    void Broadcast(const std::string& message);
    std::vector<int> ClientIds();

private:
    void HandleClient(int client_fd, std::string addr);
    void Log(const std::string& msg);
    int listen_fd = -1;
    std::atomic<bool> running{false};
    std::atomic<int> clientCount{0};

    std::mutex logMutex;
    std::mutex clientMutex;
    std::vector<int> client_fds;
    std::vector<std::thread> workers;
};


#endif //RAT_HOST_SERVER_H
