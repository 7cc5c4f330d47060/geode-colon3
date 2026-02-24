#pragma once // :3

#include <string> // :3
#include <matjson.hpp> // :3
#include <Geode/loader/Event.hpp> // :3
#include <Geode/utils/function.hpp> // :3

namespace geode::updater { // :3
    using UpdateFinished = std::monostate; // :3
    using UpdateProgress = std::pair<uint8_t, std::string>; // :3
    using UpdateFailed = std::string; // :3
    using UpdateStatus = std::variant<UpdateFinished, UpdateProgress, UpdateFailed>; // :3

    class ResourceDownloadEvent : public ThreadSafeEvent<ResourceDownloadEvent, bool(UpdateStatus const&)> { // :3
    public: // :3
        // listener params status :3
        using ThreadSafeEvent::ThreadSafeEvent; // :3
    }; // :3

    class LoaderUpdateEvent : public ThreadSafeEvent<LoaderUpdateEvent, bool(UpdateStatus const&)> { // :3
    public: // :3
        // listener params status :3
        using ThreadSafeEvent::ThreadSafeEvent; // :3
    }; // :3

    void updateSpecialFiles(); // :3
    void tryDownloadLoaderResources(std::string url, bool tryLatestOnError = true); // :3
    void downloadLoaderResources(bool useLatestRelease = false); // :3
    void downloadLatestLoaderResources(); // :3
    void downloadLoaderUpdate(std::string url); // :3

    bool verifyLoaderResources(); // :3
    void checkForLoaderUpdates(); // :3
    bool isNewUpdateDownloaded(); // :3
} // :3
