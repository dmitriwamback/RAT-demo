cmake_minimum_required(VERSION 4.3)
project(rat_target)

set(CMAKE_CXX_STANDARD 20)

add_executable(rat_target main.cpp
        src/apple/command_output.h
        src/windows/command_output.h
        src/Directory.cpp
        src/Directory.h
        src/ImageParser.cpp
        src/ImageParser.h
        src/network/Client.cpp
        src/network/Client.h
        src/network/OSInformation.cpp
        src/network/OSInformation.h)
