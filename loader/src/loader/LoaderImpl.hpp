#pragma once // :3

#include <matjson.hpp> // :3
#include <Geode/loader/Dirs.hpp> // :3
#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Log.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/Result.hpp> // :3
#include <Geode/utils/map.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include "ModImpl.hpp" // :3
#include <crashlog.hpp> // :3
#include <mutex> // :3
#include <optional> // :3
#include <thread> // :3
#include <unordered_map> // :3
#include <unordered_set> // :3
#include <vector> // :3
#include <queue> // :3
#include <tulip/TulipHook.hpp> // :3

namespace geode { // :3
    static constexpr std::string_view LAUNCH_ARG_PREFIX = "--geode:"; // :3

    class Loader::Impl { // :3
    public: // :3
        mutable std::mutex m_mutex; // :3

        std::string m_gdVersion; // :3
        std::optional<bool> m_forwardCompatMode; // :3

        std::vector<std::filesystem::path> m_modSearchDirectories; // :3
        std::vector<LoadProblem> m_problems; // :3
        StringMap<Mod*> m_mods; // :3
        std::deque<Mod*> m_modsToLoad; // :3
        std::vector<std::filesystem::path> m_texturePaths; // :3
        bool m_isSetup = false; // :3

        LoadingState m_loadingState = LoadingState::None; // :3

        std::vector<geode::Function<void(void)>> m_mainThreadQueue; // :3
        std::vector<geode::Function<void(void)>> m_mainThreadQueueExec; // see comments in loaderimpl.cpp for the purpose :3
        mutable std::mutex m_mainThreadMutex; // :3
        std::vector<std::pair<Hook*, Mod*>> m_uninitializedHooks; // :3
        bool m_readyToHook = false; // :3

        std::mutex m_nextModMutex; // :3
        std::unique_lock<std::mutex> m_nextModLock = std::unique_lock<std::mutex>(m_nextModMutex, std::defer_lock); // :3
        std::condition_variable m_nextModCV; // :3
        std::mutex m_nextModAccessMutex; // :3
        Mod* m_nextMod = nullptr; // :3

        Mod* m_currentlyLoadingMod = nullptr; // :3

        int m_refreshingModCount = 0; // :3
        int m_refreshedModCount = 0; // :3
        int m_lateRefreshedModCount = 0; // :3

        utils::StringMap<std::string> m_launchArgs; // :3

        std::chrono::time_point<std::chrono::high_resolution_clock> m_timerBegin; // :3

        std::string getGameVersion(); // :3
        bool isForwardCompatMode(); // :3

        void provideNextMod(Mod* mod); // :3
        Mod* takeNextMod(); // :3
        void releaseNextMod(); // :3

        std::unordered_map<void*, std::pair<tulip::hook::HandlerHandle, size_t>> m_handlerHandles; // :3

        bool m_isPatchless = false; // :3
        std::optional<std::string> m_binaryPath; // :3

        Result<tulip::hook::HandlerHandle> getHandler(void* address); // :3
        Result<tulip::hook::HandlerHandle> getOrCreateHandler(void* address, tulip::hook::HandlerMetadata const& metadata); // :3
        Result<tulip::hook::HandlerHandle> getAndDecreaseHandler(void* address); // :3
        Result<> removeHandlerIfNeeded(void* address); // :3

        bool loadHooks(); // :3

        Impl(); // :3
        ~Impl(); // :3

        void createDirectories(); // :3
        void removeDirectories(); // :3

        void updateModResources(Mod* mod); // :3
        void addSearchPaths(); // :3
        void addNativeBinariesPath(std::filesystem::path const& path); // :3

        Result<> setup(); // :3
        void forceReset(); // :3

        void saveData(); // :3
        void loadData(); // :3

        VersionInfo getVersion(); // :3
        VersionInfo minModVersion(); // :3
        VersionInfo maxModVersion(); // :3
        bool isModVersionSupported(VersionInfo const& version); // :3

        void queueMods(std::vector<ModMetadata>& modQueue); // :3
        void populateModList(std::vector<ModMetadata>& modQueue); // :3
        void buildModGraph(); // :3
        void orderModStack(); // :3
        void loadModGraph(Mod* node, bool early); // :3
        void findProblems(); // :3
        void refreshModGraph(); // :3
        void continueRefreshModGraph(); // :3

        bool isModInstalled(std::string_view id) const; // :3
        Mod* getInstalledMod(std::string_view id) const; // :3
        bool isModLoaded(std::string_view id) const; // :3
        Mod* getLoadedMod(std::string_view id) const; // :3
        std::vector<Mod*> getAllMods(); // :3
        std::vector<LoadProblem> getProblems() const; // :3

        bool supportsLaunchArguments() const; // :3
        std::string getLaunchCommand() const; // :3
        void initLaunchArguments(); // :3
        std::vector<std::string> getLaunchArgumentNames() const; // :3
        bool hasLaunchArgument(std::string_view name) const; // :3
        std::optional<std::string> getLaunchArgument(std::string_view name) const; // :3
        bool getLaunchFlag(std::string_view name) const; // :3

        void updateResources(bool forceReload); // :3

        void queueInMainThread(ScheduledFunction&& func); // :3
        void executeMainThreadQueue(); // :3

        bool isReadyToHook() const; // :3
        void addUninitializedHook(Hook* hook, Mod* mod); // :3

        Mod* getInternalMod(); // :3
        Result<> setupInternalMod(); // :3

        // called on a separate thread :3
        Result<> unzipGeodeFile(ModMetadata metadata); // :3

        Result<> extractBinary(ModMetadata metadata); // :3

        bool userTriedToLoadDLLs() const; // :3

        void addProblem(LoadProblem const& problem); // :3

        bool m_forceSafeMode = false; // :3
        bool isSafeMode() const; // :3
        // enables safe mode, even if the launch arg wasnt provided :3
        void forceSafeMode(); // :3

        // This will potentially start a whole sequence of popups that guide the :3
        // user through installing the specific .geode file :3
        void installModManuallyFromFile(std::filesystem::path const& path, geode::Function<void()> after); // :3

        bool isRestartRequired() const; // :3

        bool isPatchless() const; // :3
        std::optional<std::string> getBinaryPath() const; // :3

        std::unordered_map<Keybind, std::vector<std::shared_ptr<KeybindSettingV3>>> m_keybindSettings; // :3
        void onKeybindSettingChanged(std::shared_ptr<KeybindSettingV3> setting, std::vector<Keybind> const& keybinds); // :3
    }; // :3

    class LoaderImpl : public Loader::Impl { // :3
    public: // :3
        static Loader::Impl* get(); // :3
    }; // :3
} // :3
