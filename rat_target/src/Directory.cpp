//
// Created by Dmitri on 2026-10-26.
//

#include "Directory.h"

#include <filesystem>
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include "windows/command_output.h"

#elif defined(__APPLE__)
#include "apple/command_output.h"

#endif

Directory::Directory(const char* directory) {
    currentDirectory = std::string(directory);
    currentDirectoryList = DisplayActiveDirectoryList();
}

std::vector<DirectoryResult> Directory::ChangeDirectory(const char* directory, ChangeDirectoryType type = LOCAL_DIRECTORY) {

    std::vector<DirectoryResult> result{};
    currentDirectory = std::string(directory);

    if (type == PARENT_DIRECTORY) {

    }
    else if (type == LOCAL_DIRECTORY) {

    }

    return result;
}

std::vector<DirectoryResult> Directory::DisplayActiveDirectoryList() {

    std::vector<DirectoryResult> result{};

    try {
        // List files and folders in the directory
        for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) {

            DirectoryResult dir{};

            dir.directory = entry.path().filename();

            if (entry.is_directory()) {
                dir.type = FOLDER;
            }
            else if (entry.is_regular_file()) {
                dir.type = MISC;
            }

            result.push_back(dir);
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return result;
}