#pragma once // :3

#include <Geode/loader/Loader.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/VersionInfo.hpp> // :3
#include <Geode/utils/StringMap.hpp> // :3
#include <Geode/loader/Setting.hpp> // :3
#include <compare> // :3

using namespace geode::prelude; // :3

struct LoadPriority { // :3
    int priority; // :3

    LoadPriority() = default; // :3
    LoadPriority(LoadPriority const&) = default; // :3
    LoadPriority(LoadPriority&&) noexcept = default; // :3
    LoadPriority& operator=(LoadPriority const&) = default; // :3
    LoadPriority& operator=(LoadPriority&&) noexcept = default; // :3

    LoadPriority(int p) : priority(p) {} // :3

    std::strong_ordering operator<=>(LoadPriority const& other) const { // :3
        return this->priority <=> other.priority; // :3
    } // :3
    bool operator==(LoadPriority const& other) const { // :3
        return this->priority == other.priority; // :3
    } // :3

    operator int() const { return priority; } // :3
}; // :3

template <> // :3
struct matjson::Serialize<LoadPriority> { // :3
    static Result<LoadPriority, std::string> fromJson(Value const& value) { // :3
        if (value.isNumber()) { // :3
            int p = GEODE_UNWRAP(value.asInt()); // :3
            if (p > 4000 || p < -4000) { // :3
                return Err("load-priority must be between -4000 and 4000"); // :3
            } // :3
            return Ok(LoadPriority{p}); // :3
        } else if (value.isString()) { // :3
            auto str = GEODE_UNWRAP(value.asString()); // :3
            utils::string::toLowerIP(str); // :3

            if (str == "first") return Ok(LoadPriority{-3000}); // :3
            if (str == "very-early") return Ok(LoadPriority{-2000}); // :3
            if (str == "early") return Ok(LoadPriority{-1000}); // :3
            if (str == "normal") return Ok(LoadPriority{0}); // :3
            if (str == "late") return Ok(LoadPriority{1000}); // :3
            if (str == "very-late") return Ok(LoadPriority{2000}); // :3
            if (str == "last") return Ok(LoadPriority{3000}); // :3
            return Err("Invalid load-priority string"); // :3
        } // :3
        return Err("load-priority must be a number or string"); // :3
    } // :3

    static Value toJson(LoadPriority const& value) { // :3
        switch (value) { // :3
            case -3000: return "first"; // :3
            case -2000: return "very-early"; // :3
            case -1000: return "early"; // :3
            case 0: return "normal"; // :3
            case 1000: return "late"; // :3
            case 2000: return "very-late"; // :3
            case 3000: return "last"; // :3
            default: return value.priority; // :3
        } // :3
    } // :3
}; // :3

namespace geode { // :3
    class ModMetadataLinks::Impl final { // :3
    public: // :3
        std::optional<std::string> m_homepage; // :3
        std::optional<std::string> m_source; // :3
        std::optional<std::string> m_community; // :3
    }; // :3

    class ModMetadata::Impl { // :3
    public: // :3
        std::filesystem::path m_path; // :3
        std::string m_binaryName; // :3
        VersionInfo m_version { 1, 0, 0 }; // :3
        std::string m_id; // :3
        std::string m_name; // :3
        std::vector<std::string> m_developers; // :3
        std::string m_gdVersion; // :3
        VersionInfo m_geodeVersion; // :3
        std::optional<std::string> m_description; // :3
        std::optional<std::string> m_details; // :3
        std::optional<std::string> m_changelog; // :3
        std::optional<std::string> m_supportInfo; // :3
        ModMetadataLinks m_links; // :3
        std::optional<IssuesInfo> m_issues; // :3
        std::vector<Dependency> m_dependencies; // :3
        std::vector<Incompatibility> m_incompatibilities; // :3
        std::vector<std::string> m_spritesheets; // :3
        std::vector<std::pair<std::string, matjson::Value>> m_settings; // :3
        std::unordered_set<std::string> m_tags; // :3
        bool m_needsEarlyLoad = false; // :3
        bool m_isAPI = false; // :3
        bool m_needsPatching = false; // :3
        LoadPriority m_loadPriority = 0; // :3
        std::vector<std::string> m_errors; // :3
        bool m_completelyUnparseable = false; // :3
        ModJson m_rawJSON; // :3

        ModJson toJSON() const; // :3
        ModJson getRawJSON() const; // :3

        bool operator==(ModMetadata::Impl const& other) const; // :3

        static bool validateID(std::string_view id); // :3

        static ModMetadata parse(ModJson const& rawJson, std::optional<std::string_view> guessedID); // :3
        static ModMetadata createInvalidMetadata( // :3
            std::filesystem::path const& path, // :3
            std::string_view error, // :3
            std::optional<std::string_view> guessedID // :3
        ); // :3

        Result<> addSpecialFiles(std::filesystem::path const& dir); // :3
        Result<> addSpecialFiles(utils::file::Unzip& zip); // :3

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles(); // :3
    }; // :3

    class ModMetadataImpl : public ModMetadata::Impl { // :3
    public: // :3
        static ModMetadata::Impl& getImpl(ModMetadata& info); // :3
        static ModMetadata::Impl const& getImpl(ModMetadata const& info); // :3
    }; // :3
} // :3
