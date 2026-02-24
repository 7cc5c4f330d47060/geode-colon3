#pragma once // :3

#include <string> // :3
#include <matjson.hpp> // :3

namespace geode::ipc { // :3
    void setup(); // :3
    matjson::Value processRaw(void* rawHandle, std::string_view buffer); // :3
} // :3
