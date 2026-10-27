#include <iostream>
#include <thread>

#include "src/network/Server.h"

int main() {

    std::string port = "8080";
    Server server;

    if (!server.Start(port)) {
        std::cerr << "Failed to start the server." << std::endl;
        return 1;
    }

    std::thread runner(&Server::Run, &server);

    std::string line;
    while (std::getline(std::cin, line) && line != "quit") {

        std::istringstream ss(line);
        std::string cmd;

        ss >> cmd;
        std::cout << "command sent: " << cmd << std::endl;
        server.Broadcast(cmd);
    }

    server.Stop();
    runner.join();
}
