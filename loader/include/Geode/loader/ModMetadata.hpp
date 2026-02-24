#pragma once // :3

#include <Geode/Result.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include "../utils/VersionInfo.hpp" // :3
#include "Types.hpp" // :3
#include <filesystem> // :3
#include <matjson.hpp> // :3
#include <memory> // :3
#include <string> // :3

namespace geode { // :3
    namespace utils::file { // :3
        class Unzip; // :3
    } // :3

    class ModMetadataImpl; // :3

    class GEODE_DLL ModMetadataLinks final { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

        friend class ModMetadataImpl; // :3

    public: // :3
        ModMetadataLinks(); // :3
        ModMetadataLinks(ModMetadataLinks const& other); // :3
        ModMetadataLinks(ModMetadataLinks&& other) noexcept; // :3
        ModMetadataLinks& operator=(ModMetadataLinks const& other); // :3
        ModMetadataLinks& operator=(ModMetadataLinks&& other) noexcept; // :3
        ~ModMetadataLinks(); // :3

        /**
         * Get the URL for the home website for this mod
         */ // :3
        std::optional<std::string> getHomepageURL() const; // :3
        /**
         * Get the URL for the source code repository for this mod
         */ // :3
        std::optional<std::string> getSourceURL() const; // :3
        /**
         * Get the URL for the community page (Discord server etc.) for this mod
         */ // :3
        std::optional<std::string> getCommunityURL() const; // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
        Impl* getImpl(); // :3
#endif // :3
    }; // :3

    /**
     * Represents all the data gather-able
     * from mod.json
     */ // :3
    class GEODE_DLL ModMetadata final { // :3
        class Impl; // :3
        std::unique_ptr<Impl> m_impl; // :3

    public: // :3
        ModMetadata(); // :3
        explicit ModMetadata(std::string id); // :3
        ModMetadata(ModMetadata const& other); // :3
        ModMetadata(ModMetadata&& other) noexcept; // :3
        ModMetadata& operator=(ModMetadata const& other); // :3
        ModMetadata& operator=(ModMetadata&& other) noexcept; // :3
        ~ModMetadata(); // :3

        class GEODE_DLL Dependency final { // :3
            class Impl; // :3
            std::unique_ptr<Impl> m_impl; // :3

        public: // :3
            Dependency(); // :3
            Dependency(Dependency const& other); // :3
            Dependency(Dependency&& other) noexcept; // :3
            Dependency& operator=(Dependency const& other); // :3
            Dependency& operator=(Dependency&& other) noexcept; // :3
            ~Dependency(); // :3

            std::string const& getID() const; // :3
            void setID(std::string value); // :3
            ComparableVersionInfo const& getVersion() const; // :3
            void setVersion(ComparableVersionInfo value); // :3
            bool isRequired() const; // :3
            void setRequired(bool value); // :3
            Mod* getMod() const; // :3
            void setMod(Mod* mod); // :3
            matjson::Value const& getSettings() const; // :3
            void setSettings(matjson::Value value); // :3
            [[nodiscard]] bool isResolved() const; // :3
        }; // :3

        class GEODE_DLL Incompatibility final { // :3
            class Impl; // :3
            std::unique_ptr<Impl> m_impl; // :3

        public: // :3
            Incompatibility(); // :3
            Incompatibility(Incompatibility const& other); // :3
            Incompatibility(Incompatibility&& other) noexcept; // :3
            Incompatibility& operator=(Incompatibility const& other); // :3
            Incompatibility& operator=(Incompatibility&& other) noexcept; // :3
            ~Incompatibility(); // :3

            std::string const& getID() const; // :3
            void setID(std::string value); // :3
            ComparableVersionInfo const& getVersion() const; // :3
            void setVersion(ComparableVersionInfo value); // :3
            bool isBreaking() const; // :3
            void setBreaking(bool value); // :3
            Mod* getMod() const; // :3
            void setMod(Mod* mod); // :3
            [[nodiscard]] bool isResolved() const; // :3
        }; // :3

        class GEODE_DLL IssuesInfo final { // :3
            class Impl; // :3
            std::unique_ptr<Impl> m_impl; // :3

        public: // :3
            IssuesInfo(); // :3
            IssuesInfo(IssuesInfo const& other); // :3
            IssuesInfo(IssuesInfo&& other) noexcept; // :3
            IssuesInfo& operator=(IssuesInfo const& other); // :3
            IssuesInfo& operator=(IssuesInfo&& other) noexcept; // :3
            ~IssuesInfo(); // :3

            std::string const& getInfo() const; // :3
            void setInfo(std::string value); // :3
            std::optional<std::string> const& getURL() const; // :3
            void setURL(std::optional<std::string> value); // :3
        }; // :3

        /**
         * Path to the mod file
         */ // :3
        [[nodiscard]] std::filesystem::path const& getPath() const; // :3
        /**
         * Name of the platform binary within
         * the mod zip
         */ // :3
        [[nodiscard]] ZStringView getBinaryName() const; // :3
        /**
         * Mod Version. Should follow semantic versioning.
         */ // :3
        [[nodiscard]] VersionInfo getVersion() const; // :3
        /**
         * Human-readable ID of the Mod. Should be in the format
         * "developer.mod". May only contain lowercase ASCII characters,
         * numbers, dashes, underscores, and a single separating dot
         */ // :3
        [[nodiscard]] ZStringView getID() const; // :3
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */ // :3
        [[nodiscard]] ZStringView getName() const; // :3
        /**
         * The developers of this mod
         */ // :3
        [[nodiscard]] std::vector<std::string> const& getDevelopers() const; // :3
        /**
         * Short & concise description of the
         * mod.
         */ // :3
        [[nodiscard]] std::optional<std::string> const& getDescription() const; // :3
        /**
         * Detailed description of the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */ // :3
        [[nodiscard]] std::optional<std::string> const& getDetails() const; // :3
        /**
         * Changelog for the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */ // :3
        [[nodiscard]] std::optional<std::string> const& getChangelog() const; // :3
        /**
         * Support info for the mod; this means anything to show ways to
         * support the mod's development, like donations. Written in Markdown
         * (see MDTextArea for more info)
         */ // :3
        [[nodiscard]] std::optional<std::string> const& getSupportInfo() const; // :3
        /**
         * Get the links (related websites / servers / etc.) for this mod
         */ // :3
        ModMetadataLinks const& getLinks() const; // :3
        /**
         * Info about where users should report issues and request help
         */ // :3
        [[nodiscard]] std::optional<IssuesInfo> const& getIssues() const; // :3
        /**
         * Dependencies
         */ // :3
        [[nodiscard]] std::vector<Dependency> const& getDependencies() const; // :3
        /**
         * Incompatibilities
         */ // :3
        [[nodiscard]] std::vector<Incompatibility> const& getIncompatibilities() const; // :3
        /**
         * Mod spritesheet names
         */ // :3
        [[nodiscard]] std::vector<std::string> const& getSpritesheets() const; // :3
        /**
         * Mod settings
         * @note Not a map because insertion order must be preserved
         */ // :3
        [[nodiscard]] std::vector<std::pair<std::string, matjson::Value>> const& getSettings() const; // :3
        /**
         * Get the tags for this mod
         */ // :3
        [[nodiscard]] std::unordered_set<std::string> const& getTags() const; // :3
        /**
         * Whether this mod has to be loaded before the loading screen or not
         */ // :3
        [[nodiscard]] bool needsEarlyLoad() const; // :3
        /**
         * Whether this mod is an API or not
         */ // :3
        [[nodiscard]] bool isAPI() const; // :3
        /**
         * Whether this mod requires runtime memory patching
         */ // :3
        [[nodiscard]] bool needsPatching() const; // :3

        /**
         * Gets the target GD version for the current platform.
         * Returns nullopt if mod targets any GD version.
        */ // :3
        [[nodiscard]] std::optional<std::string> getGameVersion() const; // :3

        /**
         * Gets the target Geode version for the current platform.
        */ // :3
        [[nodiscard]] VersionInfo getGeodeVersion() const; // :3

        /**
         * Gets the load priority of this mod.
         */ // :3
        [[nodiscard]] int getLoadPriority() const; // :3

        /**
         * Checks if mod can be installed on the current GD version.
         * Returns Ok() if it can, Err explaining why not otherwise.
        */ // :3
        Result<> checkGameVersion() const; // :3
        /**
         * Checks if mod can be installed on the current Geode version.
         * Returns Ok() if it can, Err explaining why not otherwise.
        */ // :3
        Result<> checkGeodeVersion() const; // :3
        /**
         * Checks if mod can be installed on the current GD & Geode version.
         * Returns Ok() if it can, Err explaining why not otherwise.
        */ // :3
        Result<> checkTargetVersions() const; // :3

        /**
         * Check if this `ModMetadata` had parsing errors (or was completely
         * unparseable)
         */ // :3
        bool hasErrors() const; // :3
        std::vector<std::string> const& getErrors() const; // :3
        /**
         * Check if this `ModMetadata` was completely unparseable, i.e. created
         * from an invalid ZIP file etc.
         */ // :3
        bool wasCompletelyUnparseable() const; // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
        void setPath(std::filesystem::path value); // :3
        void setBinaryName(std::string value); // :3
        void setVersion(VersionInfo value); // :3
        void setID(std::string value); // :3
        void setName(std::string value); // :3
        void setDeveloper(std::string value); // :3
        void setDevelopers(std::vector<std::string> value); // :3
        void setDescription(std::optional<std::string> value); // :3
        void setDetails(std::optional<std::string> value); // :3
        void setChangelog(std::optional<std::string> value); // :3
        void setSupportInfo(std::optional<std::string> value); // :3
        void setRepository(std::optional<std::string> value); // :3
        void setIssues(std::optional<IssuesInfo> value); // :3
        void setDependencies(std::vector<Dependency> value); // :3
        void setIncompatibilities(std::vector<Incompatibility> value); // :3
        void setSpritesheets(std::vector<std::string> value); // :3
        void setSettings(std::vector<std::pair<std::string, matjson::Value>> value); // :3
        void setTags(std::unordered_set<std::string> value); // :3
        void setNeedsEarlyLoad(bool value); // :3
        void setIsAPI(bool value); // :3
        void setGameVersion(std::string value); // :3
        void setGeodeVersion(VersionInfo value); // :3
        ModMetadataLinks& getLinksMut(); // :3
#endif // :3

        /**
         * Create `ModMetadata` from a `.geode` package. Note that this
         * function may fail if the JSON data is wrong; make sure to check
         * `hasErrors()` afterwards on the `ModMetadata`!
         */ // :3
        static ModMetadata createFromGeodeFile(std::filesystem::path const& path); // :3
        /**
         * Create `ModMetadata` from a parsed json document. Note that this
         * function may fail if the JSON data is wrong; make sure to check
         * `hasErrors()` afterwards on the `ModMetadata`!
         */ // :3
        static ModMetadata create(ModJson const& json); // :3

        /**
         * Convert to JSON. Essentially same as getRawJSON except dynamically
         * adds runtime fields like path
         */ // :3
        [[nodiscard]] ModJson toJSON() const; // :3
        /**
         * Get the raw JSON file
         */ // :3
        [[nodiscard]] ModJson getRawJSON() const; // :3

        bool operator==(ModMetadata const& other) const; // :3

        static bool validateID(std::string_view id); // :3

        /**
         * Format a list of mod developers, truncated if there are multiple
         * developers in the same way as in the mods list
         * @note Static because this is used by InstallListCell
         */ // :3
        [[nodiscard]] static std::string formatDeveloperDisplayString( // :3
            std::vector<std::string> const& developers // :3
        ); // :3

    private: // :3
        Result<> addSpecialFiles(std::filesystem::path const& dir); // :3
        Result<> addSpecialFiles(utils::file::Unzip& zip); // :3

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles(); // :3

        friend class Loader; // :3

        friend class ModMetadataImpl; // :3
        friend class IndexItem; // :3
    }; // :3
} // :3

template <> // :3
struct matjson::Serialize<geode::ModMetadata> { // :3
    static Value toJson(geode::ModMetadata const& value) { // :3
        return Value(value.toJSON()); // :3
    } // :3
}; // :3
