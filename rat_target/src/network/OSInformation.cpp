//
// Created by Dmitri on 2026-10-26.
//

#include "OSInformation.h"

#include <cstdlib>
#include <string>

#if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>
    #include <Lmcons.h>
    #include <cstdlib>
#elif defined(__APPLE__)
    #include <sys/utsname.h>
    #include <unistd.h>
#else
    #include <sys/utsname.h>
    #include <unistd.h>
#endif

std::string OSInformation::name() {
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif
}

std::string OSInformation::architecture() {
    struct utsname info {};

#if defined(_WIN32)
    SYSTEM_INFO systemInfo{};
    GetNativeSystemInfo(&systemInfo);

    switch (systemInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            return "x86_64";
        case PROCESSOR_ARCHITECTURE_ARM64:
            return "ARM64";
        case PROCESSOR_ARCHITECTURE_INTEL:
            return "x86";
        case PROCESSOR_ARCHITECTURE_ARM:
            return "ARM";
        default:
            return "Unknown";
    }
#else
    if (uname(&info) == 0) return info.machine;

    return "Unknown";
#endif
}

std::string OSInformation::username() {
#if defined(_WIN32)
    wchar_t buffer[UNLEN + 1];
    DWORD size = UNLEN + 1;

    if (GetUserNameW(buffer, &size)) {
        // Convert the Windows wide string to UTF-8.
        int length = WideCharToMultiByte(
            CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);

        if (length > 0) {
            std::string result(length - 1, '\0');
            WideCharToMultiByte(
                CP_UTF8, 0, buffer, -1,
                result.data(), length, nullptr, nullptr);
            return result;
        }
    }

    return {};
#else
    const char* user = std::getenv("USER");
    return user ? user : "";
#endif
}

std::string OSInformation::hostname() {
#if defined(_WIN32)
    wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    if (GetComputerNameW(buffer, &size)) {
        int length = WideCharToMultiByte(
            CP_UTF8, 0, buffer, size, nullptr, 0, nullptr, nullptr);

        if (length > 0) {
            std::string result(length, '\0');
            WideCharToMultiByte(
                CP_UTF8, 0, buffer, size,
                result.data(), length, nullptr, nullptr);
            return result;
        }
    }

    return {};
#else
    char buffer[256]{};

    if (gethostname(buffer, sizeof(buffer) - 1) == 0)
        return buffer;

    return {};
#endif
}