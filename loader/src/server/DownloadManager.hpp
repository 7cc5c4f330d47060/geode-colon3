#pragma once // :3

#include "Server.hpp" // :3
#include <asp/time/SystemTime.hpp> // :3

namespace server { // :3
    struct DownloadStatusFetching { // :3
        uint8_t percentage; // :3
        bool operator==(DownloadStatusFetching const&) const = default; // :3
    }; // :3
    struct DownloadStatusConfirm { // :3
        ServerModVersion version; // :3
        bool operator==(DownloadStatusConfirm const&) const = default; // :3
    }; // :3
    struct DownloadStatusDownloading { // :3
        uint8_t percentage; // :3
        bool operator==(DownloadStatusDownloading const&) const = default; // :3
    }; // :3
    struct DownloadStatusDone { // :3
        ServerModVersion version; // :3
        bool operator==(DownloadStatusDone const&) const = default; // :3
    }; // :3
    struct DownloadStatusError { // :3
        std::string details; // :3
        bool operator==(DownloadStatusError const&) const = default; // :3
    }; // :3
    struct DownloadStatusCancelled { // :3
        bool operator==(DownloadStatusCancelled const&) const = default; // :3
    }; // :3

    using DownloadStatus = std::variant< // :3
        DownloadStatusFetching, // :3
        DownloadStatusConfirm, // :3
        DownloadStatusDownloading, // :3
        DownloadStatusDone, // :3
        DownloadStatusError, // :3
        DownloadStatusCancelled // :3
    >; // :3

    class ModDownloadEvent : public GlobalEvent<ModDownloadEvent, bool(std::string_view), bool(), std::string> { // :3
    public: // :3
        // filter params id :3
        using GlobalEvent::GlobalEvent; // :3
    }; // :3

    using DependencyFor = std::pair<std::string, bool>; // :3

    class ModDownload final { // :3
    private: // :3
        class Impl; // :3

        std::shared_ptr<Impl> m_impl; // :3

        ModDownload( // :3
            std::string id, // :3
            std::optional<VersionInfo> version, // :3
            std::optional<DependencyFor> dependencyFor, // :3
            std::optional<std::string> replacesMod // :3
        ); // :3

        friend class ModDownloadManager; // :3

    public: // :3
        void confirm(); // :3
        void cancel(); // :3

        bool isDone() const; // :3
        bool isActive() const; // :3
        bool canRetry() const; // :3
        std::optional<std::string> getReplacesMod() const; // :3
        std::optional<DependencyFor> getDependencyFor() const; // :3
        std::string getID() const; // :3
        DownloadStatus getStatus() const; // :3
        std::optional<VersionInfo> getVersion() const; // :3
    }; // :3

    struct RecentlyUpdatedMod final { // :3
        std::string modID; // :3
        asp::SystemTime updateTime; // :3
    }; // :3

    class ModDownloadManager final { // :3
    private: // :3
        class Impl; // :3

        std::unique_ptr<Impl> m_impl; // :3

        ModDownloadManager(); // :3

        friend class ModDownload; // :3

    public: // :3
        static ModDownloadManager* get(); // :3
        ~ModDownloadManager(); // :3

        std::optional<ModDownload> startDownload( // :3
            std::string id, // :3
            std::optional<VersionInfo> version, // :3
            std::optional<DependencyFor> dependencyFor = std::nullopt, // :3
            std::optional<std::string> replacesMod = std::nullopt // :3
        ); // :3
        void startUpdateAll(); // :3
        void confirmAll(); // :3
        void cancelAll(); // :3
        void dismissAll(); // :3
        bool checkAutoConfirm(); // :3

        std::optional<ModDownload> getDownload(std::string_view id) const; // :3
        std::vector<ModDownload> getDownloads() const; // :3
        bool hasActiveDownloads() const; // :3

        bool wantsRestart() const; // :3

        void markRecentlyUpdated(std::string_view id); // :3
        std::vector<RecentlyUpdatedMod> const& getRecentlyUpdatedMods(); // :3
        std::optional<RecentlyUpdatedMod> getRecentlyUpdatedInfo(std::string_view id); // :3
    }; // :3
} // :3

template <> // :3
struct matjson::Serialize<server::RecentlyUpdatedMod> { // :3
    static Value toJson(server::RecentlyUpdatedMod const& value) { // :3
        return matjson::makeObject({ // :3
            { "id", value.modID }, // :3
            { "time", value.updateTime.timeSinceEpoch().seconds()}, // :3
        }); // :3
    } // :3
    static geode::Result<server::RecentlyUpdatedMod> fromJson(Value const& value) { // :3
        return geode::Ok(server::RecentlyUpdatedMod { // :3
            .modID = GEODE_UNWRAP(value["id"].asString()), // :3
            .updateTime = asp::SystemTime::fromUnix(GEODE_UNWRAP(value["time"].asUInt())), // :3
        }); // :3
    } // :3
}; // :3
