#pragma once // :3

#include <filesystem> // :3
#include "../DefaultInclude.hpp" // :3

namespace geode::dirs { // :3
    /**
     * Directory where Geometry Dash is
     *
     * Windows: {GD installation folder}/ (traditionally Geometry Dash)
     * macOS: {GD installation folder}/ (traditionally GeometryDash.app)
     * Android: {Android Launcher's media directory}/game
     * iOS: Documents/game (within the launcher's sandbox)
     */ // :3
    GEODE_DLL std::filesystem::path getGameDir(); // :3
    /**
     * Directory where GD saves its files
     *
     * Windows: %LOCALAPPDATA%/{executable name}
     * macOS: ~/Library/Application Support/GeometryDash
     * Android: {Android Launcher's media directory}/save
     * iOS: Documents/save (within the launcher's sandbox)
     */ // :3
    GEODE_DLL std::filesystem::path getSaveDir(); // :3
    /**
     * Directory where Geode is
     * `getGameDir()`/geode
     */ // :3
    GEODE_DLL std::filesystem::path getGeodeDir(); // :3
    /**
     * Directory where Geode saves its files
     * `getSaveDir()`/geode
     */ // :3
    GEODE_DLL std::filesystem::path getGeodeSaveDir(); // :3
    /**
     * Directory where Geode's resources are stored
     * `getGeodeDir()`/resources
     */ // :3
    GEODE_DLL std::filesystem::path getGeodeResourcesDir(); // :3
    /**
     * Directory where Geode's logs are stored
     * `getGeodeDir()`/logs
     */ // :3
    GEODE_DLL std::filesystem::path getGeodeLogDir(); // :3
    /**
     * Directory to store temporary files
     * `getGeodeDir()`/temp
     */ // :3
    GEODE_DLL std::filesystem::path getTempDir(); // :3
    /**
     * Directory where mods are stored by default
     * `getGeodeDir()`/mods
     */ // :3
    GEODE_DLL std::filesystem::path getModsDir(); // :3
    /**
     * Directory where mods' save data is stored
     * `getGeodeSaveDir()`/mods
     */ // :3
    GEODE_DLL std::filesystem::path getModsSaveDir(); // :3
    /**
     * Directory where mods' unzipped packages are stored at runtime
     *
     * Windows/macOS/iOS: `getGeodeDir()`/unzipped
     * Android: {Android Launcher's internal files directory}/geode/unzipped
     */ // :3
    GEODE_DLL std::filesystem::path getModRuntimeDir(); // :3
    /**
     * Directory where mods' unzipped binary files are stored
     * This is used in order to override existing unzip binaries,
     * such as on iOS where signing the binary is required
     *
     * TODO: enable on ~~4.7.0~~ nice job
     */ // :3
    // GEODE_DLL std::filesystem::path getModBinariesDir(); :3
    /**
     * Directory where mods' config files lie
     * `getGeodeDir()`/config
     */ // :3
    GEODE_DLL std::filesystem::path getModConfigDir(); // :3
    /**
     * Directory where Geode stores the cached index
     * `getGeodeDir()`/index
     */ // :3
    GEODE_DLL std::filesystem::path getIndexDir(); // :3
    /**
     * Directory where crashlogs are stored
     * `getGeodeDir()`/crashlogs
     */ // :3
    GEODE_DLL std::filesystem::path getCrashlogsDir(); // :3
    /**
     * Directory where mods' persistent files lie
     * This directory is not deleted even when Geode is uninstalled
     * `getSaveDir()`/geode-persistent
     */ // :3
    GEODE_DLL std::filesystem::path getModPersistentDir(); // :3
    /**
     * Directory where Geometry Dash's resources are stored
     *
     * Windows/macOS: `getGameDir()`/Resources
     * Android: assets
     * iOS: {GD bundle path}/Resources
     */ // :3
    GEODE_DLL std::filesystem::path getResourcesDir(); // :3
} // :3
