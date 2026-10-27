//
// Created by Dmitri on 2026-10-26.
//

#ifndef RAT_TARGET_DIRECTORY_H
#define RAT_TARGET_DIRECTORY_H
#include <string>
#include <vector>

enum ChangeDirectoryType {
    PARENT_DIRECTORY, LOCAL_DIRECTORY
};

enum DirectoryType {
    FOLDER, IMAGE, VIDEO, TEXT_FILE, MISC
};

struct DirectoryResult {
    std::string directory;
    DirectoryType type;
};

class Directory {
public:
    Directory();
    std::vector<DirectoryResult> ChangeDirectory(const char* directory, ChangeDirectoryType type);

private:
    std::string currentDirectory;
    std::vector<DirectoryResult> currentDirectoryList;
    std::vector<DirectoryResult> DisplayActiveDirectoryList();
};


#endif //RAT_TARGET_DIRECTORY_H
