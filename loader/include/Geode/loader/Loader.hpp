#pragma once // :3

#include <filesystem> // :3
#include <Geode/Result.hpp> // :3
#include "Log.hpp" // :3
#include "ModEvent.hpp" // :3
#include "ModMetadata.hpp" // :3
#include "Types.hpp" // :3

#include <atomic> // :3
#include <matjson.hpp> // :3
#include <mutex> // :3
#include <optional> // :3
#include <string_view> // :3

namespace geode { // :3
    using ScheduledFunction = geode::Function<void()>; // :3

    struct LoadProblem { // :3
        enum class Type : uint8_t { // :3
            /// Some other fatal error (like binary loading failing) :3
            Unknown = 0, // :3
            /// This mod has an invalid .geode package :3
            InvalidGeodeFile = 1, // :3
            /// This mod is missing dependencies :3
            MissingDependencies = 2, // :3
            /// This mod is outdated (targets an old GD or Geode version) :3
            Outdated = 3, // :3
            /// This mod is explicitly incompatible with another mod :3
            HasIncompatibilities = 4, // :3
        }; // :3
        Type type; // :3
        std::variant<std::filesystem::path, ModMetadata, Mod*> cause; // :3
        /// Human-readable message (that should also suggest a fix; aka be UI-ready) :3
        std::string message; // :3

        // Outdated mods are not shown in main menu :3
        bool isProblemTheUserShouldCareAbout() const { // :3
            return type != Type::Outdated; // :3
        } // :3
    }; // :3

    class LoaderImpl; // :3

    class GEODE_DLL Loader final { // :3
    private: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3
        Loader(); // :3
        ~Loader(); // :3

    protected: // :3
        Mod* takeNextMod(); // :3

    public: // :3
        static Loader* get(); // :3

        enum class LoadingState : uint8_t { // :3
            None, // :3
            Queue, // :3
            List, // :3
            Graph, // :3
            EarlyMods, // :3
            Mods, // :3
            Problems, // :3
            Done // :3
        }; // :3

        bool isForwardCompatMode(); // :3

        void saveData(); // :3
        void loadData(); // :3

        VersionInfo getVersion(); // :3
        VersionInfo minModVersion(); // :3
        VersionInfo maxModVersion(); // :3
        bool isModVersionSupported(VersionInfo const& version); // :3

        LoadingState getLoadingState(); // :3
        bool isModInstalled(std::string_view id) const; // :3
        Mod* getInstalledMod(std::string_view id) const; // :3
        bool isModLoaded(std::string_view id) const; // :3
        Mod* getLoadedMod(std::string_view id) const; // :3
        std::vector<Mod*> getAllMods(); // :3
        std::vector<LoadProblem> getLoadProblems() const; // :3

        /**
         * Returns the available launch argument names.
         */ // :3
        std::vector<std::string> getLaunchArgumentNames() const; // :3
        /**
         * Returns whether the specified launch argument was passed in via the command line.
         * @param name The argument name
         */ // :3
        bool hasLaunchArgument(std::string_view name) const; // :3
        /**
         * Get a launch argument. These are passed into the game as command-line arguments
         * with the format `--geode:arg-name=value`.
         * @param name The argument name
         * @return The value, if present
         */ // :3
        std::optional<std::string> getLaunchArgument(std::string_view name) const; // :3
        /**
         * Get a launch argument flag. Returns whether the argument is present and its
         * value is exactly `true`.
         * @param name The argument name
         */ // :3
        bool getLaunchFlag(std::string_view name) const; // :3
        /**
         * Get and parse a launch argument value using the setting value system.
         * @param name The argument name
         */ // :3
        template <class T> // :3
        Result<T> parseLaunchArgument(std::string_view name) const { // :3
            auto str = this->getLaunchArgument(name); // :3
            if (!str.has_value()) { // :3
                return Err(fmt::format("Launch argument '{}' not found", name)); // :3
            } // :3
            auto jsonOpt = matjson::Value::parse(str.value()); // :3
            if (jsonOpt.isErr()) { // :3
                return Err(fmt::format("Parsing launch argument '{}' failed: {}", name, jsonOpt.unwrapErr())); // :3
            } // :3
            auto value = jsonOpt.unwrap(); // :3
            return value.template as<T>(); // :3
        } // :3

        void queueInMainThread(ScheduledFunction&& func); // :3

        /**
         * Returns the current game version.
         * @return The game version
         */ // :3
        std::string getGameVersion(); // :3

        /**
         * Returns whether the loader does not use dynamic patching or hooking.
         * You should use GEODE_MOD_STATIC_PATCH macro instead of Mod::patch and
         * GEODE_MOD_STATIC_HOOK macro instead of Mod::hook if that is the case.
         * Modify classes are handled automatically, and enabling/disabling hooks
         * works fine too.
         * @return True if the loader does not use dynamic patching or hooking,
         * false if it does.
         */ // :3
        bool isPatchless() const; // :3

        friend class LoaderImpl; // :3

        friend Mod* takeNextLoaderMod(); // :3
    }; // :3

    /**
     * @brief Queues a function to run on the main thread
     *
     * @param func the function to queue
    */ // :3
    inline void queueInMainThread(ScheduledFunction&& func) { // :3
        Loader::get()->queueInMainThread(std::move(func)); // :3
    } // :3

    /**
     * @brief Take the next mod to load
     *
     * @return Mod* The next mod to load
    */ // :3
    inline Mod* takeNextLoaderMod() { // :3
        return Loader::get()->takeNextMod(); // :3
    } // :3
} // :3
