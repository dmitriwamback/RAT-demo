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
    const char* currentDirectory = "";
    std::string path;

    path = std::filesystem::current_path();

    Directory directory = Directory(path.c_str());
    OSInformation osInformation = OSInformation();

    std::cout << osInformation.name() << std::endl;
    std::cout << osInformation.architecture() << std::endl;
    std::cout << osInformation.hostname() << std::endl;
    std::cout << osInformation.username() << std::endl;


    Client client;

    //std::string result = execute("ls");
    //std::cout << result << std::endl;
}