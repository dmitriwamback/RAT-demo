#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include "src/windows/command_output.h"

#elif defined(__APPLE__)
#include "src/apple/command_output.h"

#endif

int main() {



    // Sending OS information
#if defined(_WIN32)
    // windows


#elif defined(__APPLE__)
    // mac


#endif

    std::string result = execute("ls");
    std::cout << result << std::endl;
}