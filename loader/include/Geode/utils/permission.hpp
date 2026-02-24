#pragma once // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <string_view> // :3

namespace geode::utils::permission { // :3
    enum class Permission { // :3
        ReadAllFiles = 0x3, // maintain compatibility with some older mods :3
        RecordAudio // :3
    }; // :3

    /**
     * Request whether the given permission is granted to Geode by the operating system.
     * @param permission The permission
    */ // :3
    bool GEODE_DLL getPermissionStatus(Permission permission); // :3

    /**
     * Request a permission to be granted by the operating system.
     * @param permission The permission
     * @param callback The callback, passed value is 'true' if permission was granted and 'false' otherwise.
     */ // :3
    void GEODE_DLL requestPermission(Permission permission, geode::Function<void(bool)> callback); // :3
} // :3