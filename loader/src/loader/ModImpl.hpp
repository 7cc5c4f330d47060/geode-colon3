#pragma once // :3

#include <matjson.hpp> // :3
#include "ModPatch.hpp" // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3

namespace geode { // :3
    class Mod::Impl { // :3
    public: // :3
        Mod* m_self; // :3
        /**
         * Mod metadata
         */ // :3
        ModMetadata m_metadata; // :3
        /**
         * Platform-specific info
         */ // :3
        PlatformInfo* m_platformInfo = nullptr; // :3
        /**
         * Hooks owned by this mod
         */ // :3
        std::vector<std::shared_ptr<Hook>> m_hooks; // :3
        /**
         * Patches owned by this mod
         */ // :3
        std::vector<std::shared_ptr<Patch>> m_patches; // :3
        /**
         * Whether the mod is loaded or not
         */ // :3
        bool m_loaded = false; // :3
        /**
         * Mod temp directory name
         */ // :3
        std::filesystem::path m_tempDirName; // :3
        /**
         * Mod save directory name
         */ // :3
        std::filesystem::path m_saveDirPath; // :3
        /**
         * Pointers to mods that depend on this Mod.
         * Makes it possible to enable / disable them automatically,
         * when their dependency is disabled.
         */ // :3
        std::vector<Mod*> m_dependants; // :3
        /**
         * Saved values
         */ // :3
        matjson::Value m_saved = matjson::Value(); // :3
        /**
         * Setting values. This is behind unique_ptr for interior mutability
         */ // :3
        std::unique_ptr<ModSettingsManager> m_settings = nullptr; // :3
        /**
         * Whether the mod resources are loaded or not
         */ // :3
        bool m_resourcesLoaded = false; // :3
        /**
         * Whether logging is enabled for this mod
         */ // :3
        bool m_loggingEnabled = true; // :3
        /**
         * The minimum log level for this mod
         */ // :3
        Severity m_logLevel = Severity::Debug; // :3
        std::unordered_map<std::string, char const*> m_expandedSprites; // :3
        bool m_isCurrentlyLoading = false; // :3
        ModRequestedAction m_requestedAction = ModRequestedAction::None; // :3
        std::optional<LoadProblem> m_problem; // :3

        Impl(Mod* self, ModMetadata const& metadata); // :3
        ~Impl(); // :3
        Impl(Impl const&) = delete; // :3
        Impl(Impl&&) = delete; // :3

        Result<> setup(); // :3

        Result<> loadPlatformBinary(); // :3
        Result<> createTempDir(); // :3

        ZStringView getID() const; // :3
        ZStringView getName() const; // :3
        std::vector<std::string> const& getDevelopers() const; // :3
        std::optional<std::string> const& getDescription() const; // :3
        std::optional<std::string> const& getDetails() const; // :3
        std::filesystem::path getPackagePath() const; // :3
        VersionInfo getVersion() const; // :3
        bool isLoaded() const; // :3
        bool isInternal() const; // :3
        bool needsEarlyLoad(std::vector<Mod*>& checked) const; // :3
        ModMetadata const& getMetadata() const; // :3
        std::filesystem::path getTempDir() const; // :3
        std::filesystem::path getBinaryPath() const; // :3

        /**
         * If a mod should be considered ephemeral, in which case it will not attempt to save/load its data.
         * Currently used for the invalid mod objects.
         */ // :3
        bool isEphemeral() const; // :3

        matjson::Value& getSaveContainer(); // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
        void setMetadata(ModMetadata const& metadata); // :3
        std::vector<Mod*> getDependants() const; // :3
#endif // :3

        Result<> saveData(); // :3
        Result<> loadData(); // :3

        std::filesystem::path getSaveDir() const; // :3
        std::filesystem::path getConfigDir(bool create = true) const; // :3
        std::filesystem::path getPersistentDir(bool create = true) const; // :3

        bool hasSettings() const; // :3
        std::vector<std::string> getSettingKeys() const; // :3
        bool hasSetting(std::string_view key) const; // :3

        std::string getLaunchArgumentName(std::string_view name) const; // :3
        std::vector<std::string> getLaunchArgumentNames() const; // :3
        bool hasLaunchArgument(std::string_view name) const; // :3
        std::optional<std::string> getLaunchArgument(std::string_view name) const; // :3
        bool getLaunchFlag(std::string_view name) const; // :3

        Result<Hook*> claimHook(std::shared_ptr<Hook> hook); // :3
        Result<> disownHook(Hook* hook); // :3
        [[nodiscard]] std::vector<Hook*> getHooks() const; // :3

        Result<Patch*> claimPatch(std::shared_ptr<Patch> patch); // :3
        Result<> disownPatch(Patch* patch); // :3
        [[nodiscard]] std::vector<Patch*> getPatches() const; // :3

        Result<> enable(); // :3
        Result<> disable(); // :3
        Result<> uninstall(bool deleteSaveData = false); // :3
        bool isUninstalled() const; // :3

        // 1.3.0 additions :3
        ModRequestedAction getRequestedAction() const; // :3

        bool depends(std::string_view id) const; // :3
        Result<> updateDependencies(); // :3
        bool hasUnresolvedDependencies() const; // :3
        bool hasUnresolvedIncompatibilities() const; // :3

        Result<> loadBinary(); // :3

        std::string expandSpriteName(std::string_view name); // :3
        ModJson getRuntimeInfo() const; // :3

        bool isLoggingEnabled() const; // :3
        void setLoggingEnabled(bool enabled); // :3
        Severity getLogLevel() const; // :3
        void setLogLevel(Severity level); // :3

        bool shouldLoad() const; // :3
        bool isCurrentlyLoading() const; // :3

        int getLoadPriority() const; // :3

        bool isPinned() const; // :3
        void setPinned(bool pinned); // :3
    }; // :3

    class ModImpl : public Mod::Impl { // :3
    public: // :3
        static Mod::Impl* get(); // :3

        static Mod::Impl* getImpl(Mod* mod); // :3
    }; // :3
} // :3
