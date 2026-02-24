#pragma once // :3

#include <Geode/Loader.hpp> // :3
#include <Geode/utils/StringBuffer.hpp> // :3
#include <filesystem> // :3
#include <string> // :3

/**
 * Platform-specific crashlog functions. Used by the loader
 */ // :3
namespace crashlog { // :3
    using Buffer = geode::utils::StringBuffer<1>; // :3

    /**
     * Setup platform-specific crashlog handler
     * @returns True if the handler was successfully installed, false otherwise
     */ // :3
    bool GEODE_DLL setupPlatformHandler(); // :3

    /**
     * Setup platform-specific crashlog handler for post-launch
     */ // :3
    void GEODE_DLL setupPlatformHandlerPost(); // :3

    /**
     * Check if previous launch of GD crashed unexpectedly
     * @returns True if the launch crashed, false otherwise or if indeterminate
     */ // :3
    bool GEODE_DLL didLastLaunchCrash(); // :3
    /**
     * Get the directory where crashlogs are saved on the current platform
     * @returns Path to the directory, or an empty string if the platform does
     * not support crash logs
     */ // :3
    std::filesystem::path GEODE_DLL getCrashLogDirectory(); // :3


    std::string GEODE_DLL writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers); // :3

    std::string writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers, std::filesystem::path& outCrashlogPath); // :3

    std::string getDateString(bool filesafe); // :3

    void GEODE_DLL printGeodeInfo(Buffer& stream); // :3
    void GEODE_DLL printMods(Buffer& stream); // :3

    struct FunctionBinding { // :3
        std::string name; // :3
        uintptr_t offset; // :3
    }; // :3

    void updateFunctionBindings(); // :3
    std::string_view GEODE_DLL lookupClosestFunction(uintptr_t& address); // :3
    std::string_view GEODE_DLL lookupFunctionByOffset(uintptr_t address); // :3
} // :3

template <> // :3
struct matjson::Serialize<crashlog::FunctionBinding> { // :3
    static geode::Result<crashlog::FunctionBinding> fromJson(Value const& value) { // :3
        return geode::Ok(crashlog::FunctionBinding{ // :3
            GEODE_UNWRAP(value[1].asString()), // :3
            GEODE_UNWRAP(value[0].as<uintptr_t>()) // :3
        }); // :3
    } // :3

    static Value toJson(crashlog::FunctionBinding const& binding) { // :3
        auto arr = Value::array(); // :3
        arr.push(binding.offset); // :3
        arr.push(binding.name); // :3
        return arr; // :3
    } // :3
}; // :3