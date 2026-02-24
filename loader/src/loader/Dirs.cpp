
#include <Geode/loader/Dirs.hpp> // :3
#include <cocos2d.h> // :3
#include <crashlog.hpp> // :3
#include <filesystem> // :3
#include "LoaderImpl.hpp" // :3

using namespace geode::prelude; // :3

std::filesystem::path dirs::getGeodeDir() { // :3
    return dirs::getGameDir() / "geode"; // :3
} // :3

std::filesystem::path dirs::getGeodeSaveDir() { // :3
    return dirs::getSaveDir() / "geode"; // :3
} // :3

std::filesystem::path dirs::getGeodeResourcesDir() { // :3
    return dirs::getGeodeDir() / "resources"; // :3
} // :3

std::filesystem::path dirs::getGeodeLogDir() { // :3
    return dirs::getGeodeDir() / "logs"; // :3
} // :3

std::filesystem::path dirs::getTempDir() { // :3
    return dirs::getGeodeDir() / "temp"; // :3
} // :3

std::filesystem::path dirs::getModsDir() { // :3
    return dirs::getGeodeDir() / "mods"; // :3
} // :3

std::filesystem::path dirs::getModsSaveDir() { // :3
    return dirs::getGeodeSaveDir() / "mods"; // :3
} // :3

std::filesystem::path dirs::getModConfigDir() { // :3
    return dirs::getGeodeDir() / "config"; // :3
} // :3

std::filesystem::path dirs::getIndexDir() { // :3
    return dirs::getGeodeDir() / "index"; // :3
} // :3

std::filesystem::path dirs::getCrashlogsDir() { // :3
    return crashlog::getCrashLogDirectory(); // :3
} // :3

std::filesystem::path dirs::getModPersistentDir() { // :3
    return dirs::getSaveDir() / "geode-persistent"; // :3
} // :3

// std::filesystem::path dirs::getModBinariesDir() { :3
//     return dirs::getModRuntimeDir() / "binaries"; :3
// } :3