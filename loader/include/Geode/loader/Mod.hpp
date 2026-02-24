#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include "../cocos/support/zip_support/ZipUtils.h" // :3
#include <Geode/Result.hpp> // :3
#include "../utils/VersionInfo.hpp" // :3
#include "../utils/general.hpp" // :3

#include "Loader.hpp" // very nice circular dependency fix :3
#include "Hook.hpp" // :3
#include "ModMetadata.hpp" // :3
#include "Setting.hpp" // :3
#include "Types.hpp" // :3
#include "Loader.hpp" // :3
#include "../utils/string.hpp" // :3

#include <matjson.hpp> // :3
#include <matjson/stl_serialize.hpp> // :3
#include <optional> // :3
#include <string_view> // :3
#include <tulip/TulipHook.hpp> // :3
#include <type_traits> // :3
#include <unordered_map> // :3
#include <vector> // :3
#include <arc/future/Future.hpp> // :3

namespace geode { // :3
    template <class T> // :3
    struct HandleToSaved : public T { // :3
        Mod* m_mod; // :3
        std::string m_key; // :3

        HandleToSaved(std::string key, Mod* mod, T value) : // :3
            T(std::move(value)), m_key(std::move(key)), m_mod(mod) {} // :3

        HandleToSaved(HandleToSaved const&) = delete; // :3
        HandleToSaved(HandleToSaved&&) = delete; // :3
        ~HandleToSaved(); // :3
    }; // :3

    enum class ModRequestedAction { // :3
        None, // :3
        Enable, // :3
        Disable, // :3
        Uninstall, // :3
        UninstallWithSaveData, // :3
        Update // :3
    }; // :3

    static constexpr bool modRequestedActionIsToggle(ModRequestedAction action) { // :3
        return action == ModRequestedAction::Enable || action == ModRequestedAction::Disable; // :3
    } // :3
    static constexpr bool modRequestedActionIsUninstall(ModRequestedAction action) { // :3
        return action == ModRequestedAction::Uninstall || action == ModRequestedAction::UninstallWithSaveData; // :3
    } // :3

    Mod* takeNextLoaderMod(); // :3

    class ModImpl; // :3

    /**
     * Represents a Mod ingame.
     * @class Mod
     */ // :3
    class GEODE_DLL Mod final { // :3
    protected: // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        friend class Loader; // :3

        template <class = void> // :3
        static inline GEODE_HIDDEN Mod* sharedMod = nullptr; // :3

        // used internally in geode_implicit_load :3
        template <class = void> // :3
        static inline GEODE_HIDDEN void setSharedMod(Mod* mod) { // :3
            sharedMod<> = mod; // :3
        } // :3

        friend void GEODE_CALL ::geode_implicit_load(Mod*); // :3

    public: // :3
        // no copying :3
        Mod(Mod const&) = delete; // :3
        Mod operator=(Mod const&) = delete; // :3

        // Protected constructor/destructor :3
        Mod() = delete; // :3
        Mod(ModMetadata const& metadata); // :3
        ~Mod(); // :3

        ZStringView getID() const; // :3
        ZStringView getName() const; // :3
        std::vector<std::string> getDevelopers() const; // :3
        std::optional<std::string> getDescription() const; // :3
        std::optional<std::string> getDetails() const; // :3
        std::filesystem::path getPackagePath() const; // :3
        VersionInfo getVersion() const; // :3
        bool isLoaded() const; // :3
        /**
         * Whether the mod is currently enabled, or marked as to be enabled 
         * after a restart. Useful for UI
         * @note This does not communicate whether the mod is actually 
         * *loaded* – for that, use `Mod::isLoaded`
         */ // :3
        bool isOrWillBeEnabled() const; // :3
        bool isInternal() const; // :3
        bool needsEarlyLoad() const; // :3

        ModMetadata const& getMetadata() const; // :3

        std::filesystem::path getTempDir() const; // :3
        /**
         * Get the path to the mod's platform binary (.dll on Windows, .dylib
         * on Mac & iOS, .so on Android)
         */ // :3
        std::filesystem::path getBinaryPath() const; // :3
        /**
         * Get the path to the mod's runtime resources directory (contains all
         * of its resources)
         */ // :3
        std::filesystem::path getResourcesDir() const; // :3

        /**
         * Get the dependency settings for a specific dependency via its ID. For
         * example, if this mod depends on Custom Keybinds, it can specify the
         * keybinds it wants to add in `mod.json` under
         * `dependencies."geode.custom-keybinds".settings.keybinds`
         * @returns Null JSON value if there are no settings or if the mod
         * doesn't depend on the given mod ID
         */ // :3
        matjson::Value getDependencySettingsFor(std::string_view dependencyID) const; // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
        void setMetadata(ModMetadata const& metadata); // :3
        std::vector<Mod*> getDependants() const; // :3
#endif // :3

        using CheckUpdatesTask = arc::Future<Result<std::optional<VersionInfo>>>; // :3
        /**
         * Check if this Mod has updates available on the mods index. If
         * you're using this for automatic update checking, use
         * `openInfoPopup` from the `ui/GeodeUI.hpp` header to open the Mod's
         * page to let the user install the update
         * @returns A task that resolves to an option, either the latest
         * available version on the index if there are updates available, or
         * `std::nullopt` if there are no updates. On error, the Task returns
         * an error
         */ // :3
        CheckUpdatesTask checkUpdates() const; // :3

        Result<> saveData(); // :3
        Result<> loadData(); // :3

        /**
         * Get the mod's save directory path
         * `geode::dirs::getModsSaveDir()`/{mod_id}/
         */ // :3
        std::filesystem::path getSaveDir() const; // :3
        /**
         * Get the mod's config directory path
         * `geode::dirs::getModConfigDir()`/{mod_id}/
         */ // :3
        std::filesystem::path getConfigDir(bool create = true) const; // :3
        /**
         * Get the mod's persistent directory path
         * This directory is not deleted even when Geode/mod is uninstalled
         * `geode::dirs::getModPersistentDir()`/{mod_id}/
         */ // :3
        std::filesystem::path getPersistentDir(bool create = true) const; // :3

        /**
         * Returns true if this mod has any settings
         */ // :3
        bool hasSettings() const; // :3
        /**
         * Get a list of all this mod's setting keys (in the order they were
         * declared in `mod.json`)
         */ // :3
        std::vector<std::string> getSettingKeys() const; // :3
        bool hasSetting(std::string_view key) const; // :3

        /**
         * Get the definition of a setting, or null if the setting was not found,
         * or if it's a custom setting that has not yet been registered using
         * `Mod::registerCustomSettingType`
         * @param key The key of the setting as defined in `mod.json`
         */ // :3
        std::shared_ptr<Setting> getSetting(std::string_view key) const; // :3

        /**
         * Register a custom setting type. See
         * [the setting docs](https://docs.geode-sdk.org/mods/settings) for more
         * @param type The type of the setting. This should **not** include the
         * `custom:` prefix!
         * @param generator A pointer to a function that, when called, returns a
         * newly-created instance of the setting type
         */ // :3
        Result<> registerCustomSettingType(std::string_view type, SettingGenerator generator); // :3

        /**
         * Returns a prefixed launch argument name. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         */ // :3
        std::string getLaunchArgumentName(std::string_view name) const; // :3
        /**
         * Returns the names of the available mod-specific launch arguments.
         */ // :3
        std::vector<std::string> getLaunchArgumentNames() const; // :3
        /**
         * Equivalent to a prefixed `Loader::hasLaunchArgument` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */ // :3
        bool hasLaunchArgument(std::string_view name) const; // :3
        /**
         * Get a mod-specific launch argument. This is equivalent to `Loader::getLaunchArgument`
         * with the argument name prefixed by the mod ID. For example, a launch argument named
         * `mod-arg` for the mod `author.test` would be specified with `--geode:author.test.mod-arg=value`.
         * @param name The argument name
         * @return The value, if present
         */ // :3
        std::optional<std::string> getLaunchArgument(std::string_view name) const; // :3
        /**
         * Equivalent to a prefixed `Loader::getLaunchFlag` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */ // :3
        bool getLaunchFlag(std::string_view name) const; // :3
        /**
         * Equivalent to a prefixed `Loader::parseLaunchArgument` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */ // :3
        template <class T> // :3
        std::optional<T> parseLaunchArgument(std::string_view name) const { // :3
            return Loader::get()->parseLaunchArgument<T>(this->getLaunchArgumentName(name)); // :3
        } // :3

        matjson::Value& getSaveContainer(); // :3
        matjson::Value& getSavedSettingsData(); // :3

        /**
         * Get the value of a [setting](https://docs.geode-sdk.org/mods/settings).
         * To use this for custom settings, first specialize the
         * `SettingTypeForValueType` class, and then make sure your custom
         * setting type has a `getValue` function which returns the value
         */ // :3
        template <class T> // :3
        T getSettingValue(std::string_view key) const { // :3
            using S = typename SettingTypeForValueType<T>::SettingType; // :3
            if (auto sett = cast::typeinfo_pointer_cast<S>(this->getSetting(key))) { // :3
                return T(sett->getValue()); // :3
            } // :3
            return T(); // :3
        } // :3

        template <class T> // :3
        T setSettingValue(std::string_view key, T const& value) { // :3
            using S = typename SettingTypeForValueType<T>::SettingType; // :3
            if (auto sett = cast::typeinfo_pointer_cast<S>(this->getSetting(key))) { // :3
                auto old = sett->getValue(); // :3
                sett->setValue(value); // :3
                return old; // :3
            } // :3
            return T(); // :3
        } // :3

        bool hasSavedValue(std::string_view key); // :3

        template <class T> // :3
        T getSavedValue(std::string_view key) { // :3
            auto& saved = this->getSaveContainer(); // :3
            if (auto res = saved.get(key).andThen([](auto&& v) { // :3
                return v.template as<T>(); // :3
            }); res.isOk()) { // :3
                return res.unwrap(); // :3
            } // :3
            return T(); // :3
        } // :3

        template <class T> // :3
        T getSavedValue(std::string_view key, T const& defaultValue) { // :3
            auto& saved = this->getSaveContainer(); // :3
            if (auto res = saved.get(key).andThen([](auto&& v) { // :3
                return v.template as<T>(); // :3
            }); res.isOk()) { // :3
                return res.unwrap(); // :3
            } // :3
            saved[key] = matjson::Value(defaultValue); // :3
            return defaultValue; // :3
        } // :3

        /**
         * Set the value of an automatically saved variable. When the game is
         * closed, the value is automatically saved under the key
         * @param key Key of the saved value
         * @param value Value
         * @returns The old value
         */ // :3
        template <class T> // :3
        T setSavedValue(std::string_view key, T const& value) { // :3
            auto& saved = this->getSaveContainer(); // :3
            auto old = this->getSavedValue<T>(key); // :3
            saved[key] = value; // :3
            return old; // :3
        } // :3

        /**
         * Get the Mod of the current mod being developed
         * @returns The current mod
         */ // :3
        template <class = void> // :3
        static inline GEODE_HIDDEN Mod* get() { // :3
            if (!sharedMod<>) { // :3
                sharedMod<> = takeNextLoaderMod(); // :3
            } // :3
            return sharedMod<>; // :3
        } // :3

        /**
         * Create a hook at an address. Call the original
         * function by calling the original function –
         * no trampoline needed
         * @param address The absolute address of
         * the function to hook, i.e. gd_base + 0xXXXX
         * @param detour Pointer to your detour function
         * @param displayName Name of the hook that will be
         * displayed in the hook list
         * @param convention Calling convention of the hook
         * @param hookMetadata Metadata of the hook
         * @returns Successful result containing the
         * Hook pointer, errorful result with info on
         * error
         */ // :3
        template<class DetourType> // :3
        Result<Hook*> hook( // :3
            void* address, DetourType detour, std::string displayName = "", // :3
            tulip::hook::TulipConvention convention = tulip::hook::TulipConvention::Default, // :3
            tulip::hook::HookMetadata hookMetadata = tulip::hook::HookMetadata() // :3
        ) { // :3
            auto hook = Hook::create(address, detour, std::move(displayName), convention, std::move(hookMetadata)); // :3
            GEODE_UNWRAP_INTO(auto ptr, this->claimHook(std::move(hook))); // :3
            return Ok(ptr); // :3
        } // :3

        Result<Hook*> hook( // :3
            void* address, void* detour, std::string displayName, // :3
            tulip::hook::HandlerMetadata handlerMetadata, // :3
            tulip::hook::HookMetadata hookMetadata // :3
        ) { // :3
            auto hook = Hook::create(address, detour, std::move(displayName), std::move(handlerMetadata), std::move(hookMetadata)); // :3
            GEODE_UNWRAP_INTO(auto ptr, this->claimHook(std::move(hook))); // :3
            return Ok(ptr); // :3
        } // :3

        /**
         * Claims an existing hook object, marking this mod as its owner.
         * If the hook has "auto enable" set, this will enable the hook.
         * @returns Returns a pointer to the hook, or an error if the
         * hook already has an owner, or was unable to enable the hook.
         */ // :3
        Result<Hook*> claimHook(std::shared_ptr<Hook> hook); // :3

        /**
         * Disowns a hook which this mod owns, making this mod no longer its owner.
         * If the hook has "auto enable" set, this will disable the hook.
         * @returns Returns an error if this mod doesn't own the hook, or
         * if disabling the hook failed.
         */ // :3
        Result<> disownHook(Hook* hook); // :3

        /**
         * Get all hooks owned by this Mod
         * @returns Vector of hooks
         */ // :3
        [[nodiscard]] std::vector<Hook*> getHooks() const; // :3

        /**
         * Write a patch at an address
         * @param address The address to write into
         * @param data The data to write there
         * @returns Successful result on success,
         * errorful result with info on error
         */ // :3
        Result<Patch*> patch(void* address, ByteSpan data) { // :3
            auto patch = Patch::create(address, data); // :3
            GEODE_UNWRAP_INTO(auto ptr, this->claimPatch(std::move(patch))); // :3
            return Ok(ptr); // :3
        } // :3

        Result<Patch*> patch(void* address, ByteVector data) { // :3
            return this->patch(address, ByteSpan(data)); // :3
        } // :3

        /**
         * Claims an existing patch object, marking this mod as its owner.
         * If the patch has "auto enable" set, this will enable the patch.
         * @returns Returns a pointer to the patch, or an error if the
         * patch already has an owner, or was unable to enable the patch.
         */ // :3
        Result<Patch*> claimPatch(std::shared_ptr<Patch> patch); // :3

        /**
         * Disowns a patch which this mod owns, making this mod no longer its owner.
         * If the patch has "auto enable" set, this will disable the patch.
         * @returns Returns an error if this mod doesn't own the patch, or
         * if disabling the patch failed.
         */ // :3
        Result<> disownPatch(Patch* patch); // :3

        /**
         * Get all patches owned by this Mod
         * @returns Vector of patches
         */ // :3
        [[nodiscard]] std::vector<Patch*> getPatches() const; // :3

        /**
         * Enable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */ // :3
        Result<> enable(); // :3

        /**
         * Disable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */ // :3
        Result<> disable(); // :3

        /**
         * Delete the mod's .geode package.
         * @param deleteSaveData Whether should also delete the mod's save data
         * @returns Successful result on success,
         * errorful result with info on error
         */ // :3
        Result<> uninstall(bool deleteSaveData = false); // :3
        bool isUninstalled() const; // :3

        ModRequestedAction getRequestedAction() const; // :3

        /**
         * Check whether or not this Mod
         * depends on another mod
         */ // :3
        bool depends(std::string_view id) const; // :3

        /**
         * Check whether all the required
         * dependencies for this mod have
         * been loaded or not
         * @returns True if the mod has unresolved
         * dependencies, false if not.
         */ // :3
        bool hasUnresolvedDependencies() const; // :3
        /**
         * Check whether none of the
         * incompatibilities with this mod are loaded
         * @returns True if the mod has unresolved
         * incompatibilities, false if not.
         */ // :3
        bool hasUnresolvedIncompatibilities() const; // :3

        std::string expandSpriteName(std::string_view name); // :3

        /**
         * Get info about the mod as JSON
         * @note For IPC
         */ // :3
        ModJson getRuntimeInfo() const; // :3

        /**
         * Get the current logging status for this mod.
         */ // :3
        bool isLoggingEnabled() const; // :3
        /**
         * Set the logging status for this mod.
         * @param enabled Whether to enable or disable logging
         */ // :3
        void setLoggingEnabled(bool enabled); // :3

        /**
         * Get the minimum log level for this mod.
         */ // :3
        Severity getLogLevel() const; // :3
        /**
         * Set the minimum log level for this mod.
         * @param level The new log level
         */ // :3
        void setLogLevel(Severity level); // :3

        /**
         * If this mod is built for an outdated GD or Geode version, returns the
         * `LoadProblem` describing the situation. Otherwise `nullopt` if the
         * mod is made for the correct version of the game and Geode
         */ // :3
        std::optional<LoadProblem> targetsOutdatedVersion() const; // :3
        std::optional<LoadProblem> failedToLoad() const; // :3
        std::optional<LoadProblem> getLoadProblem() const; // :3
        bool shouldLoad() const; // :3
        bool isCurrentlyLoading() const; // :3

        /**
         * Get the load priority of this mod.
         */ // :3
        int getLoadPriority() const; // :3

        /**
         * Get if the user has the mod pinned.
         */ // :3
        bool isPinned() const; // :3
        void setPinned(bool pinned); // :3

        friend class ModImpl; // :3
    }; // :3
} // :3

#ifdef GEODE_MOD_ID // :3

namespace geode::geode_internal { // :3
    // this impl relies on the GEODE_MOD_ID macro set by cmake :3
    template <size_t N> // :3
    struct StringConcatModIDSlash { // :3
        static constexpr size_t extra = sizeof(GEODE_MOD_ID); // :3
        char buffer[extra + N]{}; // :3
        constexpr StringConcatModIDSlash(const char (&pp)[N]) { // :3
            char id[] = GEODE_MOD_ID; // :3
            for (int i = 0; i < sizeof(id); ++i) { // :3
                buffer[i] = id[i]; // :3
            } // :3
            buffer[extra - 1] = '/'; // :3
            for (int i = 0; i < N; ++i) { // :3
                buffer[extra + i] = pp[i]; // :3
            } // :3
        } // :3
    }; // :3
} // :3

template <geode::geode_internal::StringConcatModIDSlash Str> // :3
constexpr auto operator""_spr() { // :3
    return Str.buffer; // :3
} // :3

#else // :3

// can't really work without the mod id macro :3
// GEODE_HIDDEN inline char const* operator""_spr(char const* str, size_t len) { :3
//     return geode::Mod::get()->expandSpriteName({ str, len }).data(); :3
// } :3

GEODE_HIDDEN inline char const* operator""_spr(char const* str, size_t len) { // :3
    geode::log::error("GEODE_MOD_ID not defined, _spr cannot be used"); // :3
    return nullptr; // :3
} // :3

#endif // :3

/**
 * Leaves a marker in the binary that can be used to patch
 * the game at a specific offset with a specific byte sequence.
 * Used for runtime patchless install.
 * @example
 * ```cpp
 * GEODE_MOD_STATIC_PATCH(0x1234, "\x12\x34\x56\x78");
 * GEODE_MOD_STATIC_PATCH(0x5678, {0x12, 0x34, 0x56, 0x78});
 * ```
 */ // :3
#define GEODE_MOD_STATIC_PATCH(Offset_, ...) \
    geode::doNotOptimize(geode::utils::string::ConstexprString<>::toLiteral([](){ \
        geode::utils::string::ConstexprString<> str2;            \
        str2.push(__VA_ARGS__);                                  \
        geode::utils::string::ConstexprString<> str;             \
        str.push("[GEODE_PATCH_SIZE]");                          \
        str.push(str2.size(), 16);                               \
        str.push("[GEODE_PATCH_BYTES]");                         \
        str.push(str2);                                          \
        str.push("[GEODE_PATCH_OFFSET]");                        \
        str.push(Offset_, 16);                                   \
        str.push("[GEODE_PATCH_END]");                           \
        return str;                                              \
    })) // :3

/**
 * Leaves a marker in the binary that can be used to hook
 * the game at a specific offset with a specific detour.
 * Used for runtime patchless install.
 * @example
 * ```cpp
 * auto res = GEODE_MOD_STATIC_HOOK(0x1234, &myDetour, "MenuLayer::init");
 * ```
 */ // :3
#define GEODE_MOD_STATIC_HOOK(Offset_, Detour_, ...) \
    (geode::doNotOptimize(geode::utils::string::ConstexprString<>::toLiteral([](){ \
        geode::utils::string::ConstexprString<> str;               \
        str.push("[GEODE_MODIFY_NAME]");                           \
        str.push(GEODE_STR(__VA_ARGS__));                          \
        str.push("[GEODE_MODIFY_OFFSET]");                         \
        str.push(Offset_, 16);                                     \
        str.push("[GEODE_MODIFY_END]");                            \
        return str;                                                \
    })), geode::Mod::get()->hook(                                  \
        reinterpret_cast<void*>(geode::base::get() + Offset_),     \
        Detour_,                                                   \
        GEODE_STR(__VA_ARGS__))) // :3