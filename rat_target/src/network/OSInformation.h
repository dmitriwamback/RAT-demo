//
// Created by Dmitri on 2026-10-26.
//

#ifndef RAT_TARGET_OSINFORMATION_H
#define RAT_TARGET_OSINFORMATION_H
#include <string>


class OSInformation {
public:
    [[nodiscard]] static std::string name();
    [[nodiscard]] static std::string architecture();

    [[nodiscard]] static std::string username();
    [[nodiscard]] static std::string hostname();
};


#endif //RAT_TARGET_OSINFORMATION_H
