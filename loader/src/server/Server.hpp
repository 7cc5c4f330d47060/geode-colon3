#pragma once // :3

#include "Geode/utils/VersionInfo.hpp" // :3
#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/web.hpp> // :3
#include <chrono> // :3
#include <matjson.hpp> // :3
#include <vector> // :3

using namespace geode::prelude; // :3

namespace server { // :3
    // todo: replace parse()s with Serialize::fromJson now that it uses Results :3

    struct ServerDateTime final { // :3
        using Clock = std::chrono::system_clock; // :3
        using Value = std::chrono::time_point<Clock>; // :3

        Value value; // :3

        static Result<ServerDateTime> parse(std::string const& str); // :3
    }; // :3

    struct ServerTag final { // :3
        size_t id; // :3
        std::string name; // :3
        std::string displayName; // :3

        static Result<ServerTag> parse(matjson::Value json); // :3
        static Result<std::vector<ServerTag>> parseList(matjson::Value json); // :3
    }; // :3

    struct ServerDeveloper final { // :3
        std::string username; // :3
        std::string displayName; // :3
        bool isOwner; // :3
    }; // :3

    struct ServerModVersion final { // :3
        ModMetadata metadata; // :3
        std::string downloadURL; // :3
        std::string hash; // :3
        size_t downloadCount; // :3

        bool operator==(ServerModVersion const&) const = default; // :3

        static Result<ServerModVersion> parse(matjson::Value json); // :3
    }; // :3

    struct ServerModUpdate final { // :3
        std::string id; // :3
        VersionInfo version; // :3

        static Result<ServerModUpdate> parse(matjson::Value json); // :3
        static Result<std::vector<ServerModUpdate>> parseList(matjson::Value json); // :3

        Mod* hasUpdateForInstalledMod() const; // :3
    }; // :3
    struct ServerModDeprecation final { // :3
        std::string id; // :3
        std::vector<std::string> by; // :3
        std::string reason; // :3

        static Result<ServerModDeprecation> parse(matjson::Value json); // :3
        static Result<std::vector<ServerModDeprecation>> parseList(matjson::Value json); // :3

        Mod* hasDeprecationForInstalledMod() const; // :3
    }; // :3
    struct ServerModUpdateAllCheck final { // :3
        std::vector<ServerModUpdate> updates; // :3
        std::vector<ServerModDeprecation> deprecations; // :3

        static Result<ServerModUpdateAllCheck> parse(matjson::Value json); // :3
    }; // :3
    struct ServerModUpdateOneCheck final { // :3
        std::optional<ServerModUpdate> update; // :3
        std::optional<ServerModDeprecation> deprecation; // :3
    }; // :3

    struct ServerModLinks final { // :3
        std::optional<std::string> community; // :3
        std::optional<std::string> homepage; // :3
        std::optional<std::string> source; // :3

        static Result<ServerModLinks> parse(matjson::Value json); // :3
    }; // :3

    struct ServerModMetadata final { // :3
        std::string id; // :3
        bool featured; // :3
        size_t downloadCount; // :3
        std::vector<ServerDeveloper> developers; // :3
        std::vector<ServerModVersion> versions; // :3
        std::unordered_set<std::string> tags; // :3
        std::optional<std::string> about; // :3
        std::optional<std::string> changelog; // :3
        std::optional<std::string> repository; // :3
        std::optional<ServerDateTime> createdAt; // :3
        std::optional<ServerDateTime> updatedAt; // :3

        static Result<ServerModMetadata> parse(matjson::Value json); // :3

        ModMetadata const& latestVersion() const; // :3
        std::string formatDevelopersToString() const; // :3
        Mod* hasUpdateForInstalledMod() const; // :3
    }; // :3

    struct ServerModsList final { // :3
        std::vector<ServerModMetadata> mods; // :3
        size_t totalModCount = 0; // :3

        static Result<ServerModsList> parse(matjson::Value json); // :3
    }; // :3

    struct ServerLoaderVersion final { // :3
        std::string version; // :3
        std::string tag; // :3
        std::string commitHash; // :3
        std::string gameVersion; // :3

        static Result<ServerLoaderVersion> parse(matjson::Value json); // :3
    }; // :3

    enum class ModsSort { // :3
        Downloads, // :3
        RecentlyUpdated, // :3
        RecentlyPublished, // :3
    }; // :3

    static const char* sortToString(ModsSort sorting); // :3

    struct ModsQuery final { // :3
        std::optional<std::string> query; // :3
        std::unordered_set<PlatformID> platforms = { GEODE_PLATFORM_TARGET }; // :3
        std::unordered_set<std::string> tags; // :3
        std::optional<bool> featured; // :3
        ModsSort sorting = ModsSort::Downloads; // :3
        std::optional<std::string> developer; // :3
        size_t page = 0; // :3
        size_t pageSize = 10; // :3

        bool operator==(ModsQuery const& other) const = default; // :3
    }; // :3

    struct ServerError final { // :3
        int code; // :3
        std::string details; // :3

        ServerError() = default; // :3

        template <class... Args> // :3
        ServerError( // :3
            int code, // :3
            fmt::string_view format, // :3
            Args&&... args // :3
        ) : code(code), details(fmt::vformat(format, fmt::make_format_args(args...))) {} // :3
    }; // :3
    struct ServerProgress { // :3
        std::string message; // :3
        std::optional<uint8_t> percentage; // :3

        ServerProgress() = default; // :3
        ServerProgress(std::string msg) : message(std::move(msg)) {} // :3
        ServerProgress(std::string msg, std::optional<uint8_t> percentage) : message(std::move(msg)), percentage(percentage) {} // :3
    }; // :3

    template <class T> // :3
    using ServerResult = Result<T, ServerError>; // :3

    template <class T> // :3
    using ServerFuture = arc::Future<ServerResult<T>>; // :3

    struct ModVersionLatest final { // :3
        bool operator==(ModVersionLatest const&) const = default; // :3
    }; // :3
    struct ModVersionMajor final { // :3
        size_t major; // :3
        bool operator==(ModVersionMajor const&) const = default; // :3
    }; // :3
    using ModVersionSpecific = VersionInfo; // :3
    using ModVersion = std::variant<ModVersionLatest, ModVersionMajor, ModVersionSpecific>; // :3

    std::string getServerAPIBaseURL(); // :3
    std::string getServerUserAgent(); // :3

    ServerFuture<ServerModsList> getMods(ModsQuery query, bool useCache = true); // :3
    ServerFuture<ServerModMetadata> getMod(std::string id, bool useCache = true); // :3
    ServerFuture<ServerModVersion> getModVersion(std::string id, ModVersion version = ModVersionLatest(), bool useCache = true); // :3
    ServerFuture<ByteVector> getModLogo(std::string id, bool useCache = true); // :3
    ServerFuture<std::vector<ServerTag>> getTags(bool useCache = true); // :3

    ServerFuture<ServerModUpdateOneCheck> checkUpdates(Mod const* mod); // :3
    ServerFuture<ServerModUpdateAllCheck> batchedCheckUpdates(std::vector<std::string> const& batch); // :3
    ServerFuture<ServerModUpdateAllCheck> checkAllUpdates(bool useCache = true); // :3

    ServerFuture<ServerLoaderVersion> getLoaderVersion(std::string tag, bool useCache = true); // :3
    ServerFuture<ServerLoaderVersion> getLatestLoaderVersion(bool useCache = true); // :3

    void clearServerCaches(bool clearGlobalCaches = false); // :3
} // :3
