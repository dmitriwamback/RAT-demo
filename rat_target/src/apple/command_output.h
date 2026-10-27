//
// Created by Dmitri on 2026-10-26.
//

#ifndef RAT_TARGET_COMMAND_OUTPUT_H
#define RAT_TARGET_COMMAND_OUTPUT_H

#include <iostream>
#include <string>
#include <memory>

inline std::string execute(const char* command) {
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed");
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}

#endif //RAT_TARGET_COMMAND_OUTPUT_H
