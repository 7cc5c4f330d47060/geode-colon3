#pragma once // :3

#include <Geode/utils/VersionInfo.hpp> // :3

namespace about { // :3
    GEODE_DLL geode::VersionInfo getLoaderVersion(); // :3
    GEODE_DLL const char* getLoaderVersionStr(); // :3
    GEODE_DLL const char* getLoaderCommitHash(); // :3
    GEODE_DLL const char* getBindingsCommitHash(); // :3
    GEODE_DLL const char* getLoaderModJson(); // :3
}; // :3
