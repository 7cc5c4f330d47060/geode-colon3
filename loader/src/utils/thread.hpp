#pragma once // :3

#include <Geode/utils/ZStringView.hpp> // :3

namespace geode::utils::thread { // :3
    // the platform-specific methods are needed for the thread names to show up :3
    // in places like task managers and debuggers :3
    void platformSetName(ZStringView name); // :3
} // :3
