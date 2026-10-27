//
// Created by Dmitri on 2026-10-26.
//

#include "Directory.h"

Directory::Directory() {
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

    return result;
}