#include <Geode/loader/Loader.hpp> // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/VersionInfo.hpp> // :3
#include <Geode/utils/file.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/general.hpp> // :3
#include <Geode/utils/random.hpp> // :3
#include <about.hpp> // :3
#include <matjson.hpp> // :3
#include <utility> // :3
#include <clocale> // :3

#include "ModMetadataImpl.hpp" // :3
#include "LoaderImpl.hpp" // :3

using namespace geode::prelude; // :3

std::optional<std::string> ModMetadataLinks::getHomepageURL() const { // :3
    return m_impl->m_homepage; // :3
} // :3
std::optional<std::string> ModMetadataLinks::getSourceURL() const { // :3
    return m_impl->m_source; // :3
} // :3
std::optional<std::string> ModMetadataLinks::getCommunityURL() const { // :3
    return m_impl->m_community; // :3
} // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
ModMetadataLinks::Impl* ModMetadataLinks::getImpl() { // :3
    return m_impl.get(); // :3
} // :3
#endif // :3

ModMetadataLinks::ModMetadataLinks() : m_impl(std::make_unique<Impl>()) {} // :3
ModMetadataLinks::ModMetadataLinks(ModMetadataLinks const& other) // :3
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {} // :3
ModMetadataLinks::ModMetadataLinks(ModMetadataLinks&& other) noexcept // :3
  : m_impl(std::move(other.m_impl)) {} // :3
ModMetadataLinks& ModMetadataLinks::operator=(ModMetadataLinks const& other) { // :3
    if (this != &other) { // :3
        m_impl = std::make_unique<Impl>(*other.m_impl); // :3
    } // :3
    return *this; // :3
} // :3
ModMetadataLinks& ModMetadataLinks::operator=(ModMetadataLinks&& other) noexcept = default; // :3
ModMetadataLinks::~ModMetadataLinks() = default; // :3

ModMetadata::Impl& ModMetadataImpl::getImpl(ModMetadata& info) { // :3
    return *info.m_impl; // :3
} // :3

ModMetadata::Impl const& ModMetadataImpl::getImpl(ModMetadata const& info) { // :3
    return *info.m_impl; // :3
} // :3

class ModMetadata::Dependency::Impl { // :3
public: // :3
    std::string id; // :3
    ComparableVersionInfo version; // :3
    bool required = true; // :3
    Mod* mod = nullptr; // :3
    matjson::Value settings; // :3
}; // :3

ModMetadata::Dependency::Dependency() // :3
  : m_impl(std::make_unique<Impl>()) {} // :3

ModMetadata::Dependency::Dependency(Dependency const& other) // :3
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {} // :3

ModMetadata::Dependency::Dependency(Dependency&& other) noexcept // :3
  : m_impl(std::move(other.m_impl)) {} // :3

ModMetadata::Dependency& ModMetadata::Dependency::operator=(Dependency const& other) { // :3
    if (this != &other) { // :3
        m_impl = std::make_unique<Impl>(*other.m_impl); // :3
    } // :3
    return *this; // :3
} // :3

ModMetadata::Dependency& ModMetadata::Dependency::operator=(Dependency&& other) noexcept = default; // :3

ModMetadata::Dependency::~Dependency() = default; // :3

std::string const& ModMetadata::Dependency::getID() const { // :3
    return m_impl->id; // :3
} // :3

void ModMetadata::Dependency::setID(std::string value) { // :3
    m_impl->id = std::move(value); // :3
} // :3

ComparableVersionInfo const& ModMetadata::Dependency::getVersion() const { // :3
    return m_impl->version; // :3
} // :3

void ModMetadata::Dependency::setVersion(ComparableVersionInfo value) { // :3
    m_impl->version = std::move(value); // :3
} // :3

bool ModMetadata::Dependency::isRequired() const { // :3
    return m_impl->required; // :3
} // :3

void ModMetadata::Dependency::setRequired(bool value) { // :3
    m_impl->required = value; // :3
} // :3

Mod* ModMetadata::Dependency::getMod() const { // :3
    return m_impl->mod; // :3
} // :3

void ModMetadata::Dependency::setMod(Mod* mod) { // :3
    m_impl->mod = mod; // :3
} // :3

matjson::Value const& ModMetadata::Dependency::getSettings() const { // :3
    return m_impl->settings; // :3
} // :3

void ModMetadata::Dependency::setSettings(matjson::Value value) { // :3
    m_impl->settings = std::move(value); // :3
} // :3

bool ModMetadata::Dependency::isResolved() const { // :3
    return // :3
        !m_impl->required || // :3
        this->getMod() && this->getMod()->isLoaded() && this->getVersion().compare(this->getMod()->getVersion()); // :3
} // :3

class ModMetadata::Incompatibility::Impl { // :3
public: // :3
    std::string id; // :3
    ComparableVersionInfo version; // :3
    bool breaking = true; // :3
    Mod* mod = nullptr; // :3
}; // :3

ModMetadata::Incompatibility::Incompatibility() // :3
  : m_impl(std::make_unique<Impl>()) {} // :3

ModMetadata::Incompatibility::Incompatibility(Incompatibility const& other) // :3
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {} // :3

ModMetadata::Incompatibility::Incompatibility(Incompatibility&& other) noexcept // :3
    : m_impl(std::move(other.m_impl)) {} // :3

ModMetadata::Incompatibility& ModMetadata::Incompatibility::operator=(Incompatibility const& other) { // :3
    if (this != &other) { // :3
        m_impl = std::make_unique<Impl>(*other.m_impl); // :3
    } // :3
    return *this; // :3
} // :3

ModMetadata::Incompatibility& ModMetadata::Incompatibility::operator=(Incompatibility&& other) noexcept = default; // :3

ModMetadata::Incompatibility::~Incompatibility() = default; // :3

std::string const& ModMetadata::Incompatibility::getID() const { // :3
    return m_impl->id; // :3
} // :3

void ModMetadata::Incompatibility::setID(std::string value) { // :3
    m_impl->id = std::move(value); // :3
} // :3

ComparableVersionInfo const& ModMetadata::Incompatibility::getVersion() const { // :3
    return m_impl->version; // :3
} // :3

void ModMetadata::Incompatibility::setVersion(ComparableVersionInfo value) { // :3
    m_impl->version = std::move(value); // :3
} // :3

bool ModMetadata::Incompatibility::isBreaking() const { // :3
    return m_impl->breaking; // :3
} // :3

void ModMetadata::Incompatibility::setBreaking(bool value) { // :3
    m_impl->breaking = value; // :3
} // :3

Mod* ModMetadata::Incompatibility::getMod() const { // :3
    return m_impl->mod; // :3
} // :3

void ModMetadata::Incompatibility::setMod(Mod* mod) { // :3
    m_impl->mod = mod; // :3
} // :3

bool ModMetadata::Incompatibility::isResolved() const { // :3
    return !m_impl->breaking || // :3
        (!this->getMod() || !this->getVersion().compare(this->getMod()->getVersion()) || !this->getMod()->shouldLoad()); // :3
} // :3

class ModMetadata::IssuesInfo::Impl { // :3
public: // :3
    std::string info; // :3
    std::optional<std::string> url; // :3
}; // :3

ModMetadata::IssuesInfo::IssuesInfo() // :3
  : m_impl(std::make_unique<Impl>()) {} // :3

ModMetadata::IssuesInfo::IssuesInfo(IssuesInfo const& other) // :3
  : m_impl(std::make_unique<Impl>(*other.m_impl)) {} // :3

ModMetadata::IssuesInfo::IssuesInfo(IssuesInfo&& other) noexcept // :3
    : m_impl(std::move(other.m_impl)) {} // :3

ModMetadata::IssuesInfo& ModMetadata::IssuesInfo::operator=(IssuesInfo const& other) { // :3
    if (this != &other) { // :3
        m_impl = std::make_unique<Impl>(*other.m_impl); // :3
    } // :3
    return *this; // :3
} // :3

ModMetadata::IssuesInfo& ModMetadata::IssuesInfo::operator=(IssuesInfo&& other) noexcept = default; // :3

ModMetadata::IssuesInfo::~IssuesInfo() = default; // :3

std::string const& ModMetadata::IssuesInfo::getInfo() const { // :3
    return m_impl->info; // :3
} // :3

void ModMetadata::IssuesInfo::setInfo(std::string value) { // :3
    m_impl->info = std::move(value); // :3
} // :3

std::optional<std::string> const& ModMetadata::IssuesInfo::getURL() const { // :3
    return m_impl->url; // :3
} // :3

void ModMetadata::IssuesInfo::setURL(std::optional<std::string> value) { // :3
    m_impl->url = std::move(value); // :3
} // :3

static std::string sanitizeDetailsData(std::string str) { // :3
    // delete CRLF :3
    return utils::string::replace(std::move(str), "\r", ""); // :3
} // :3

ModMetadata ModMetadata::Impl::parse(ModJson const& rawJson, std::optional<std::string_view> guessedID) { // :3
    ModMetadata info; // :3

    auto impl = info.m_impl.get(); // :3

    impl->m_rawJSON = rawJson; // :3

    auto checkerRoot = fmt::format( // :3
        "[{}/{}/mod.json]", // :3
        rawJson["id"].asString().unwrapOr(guessedID ? std::string(*guessedID) : std::string("unknown.mod")), // :3
        rawJson["version"].as<VersionInfo>().map( // :3
            [](VersionInfo const& v) { // :3
                return v.toVString(); // :3
            } // :3
        ).unwrapOr("v0.0.0") // :3
    ); // :3

    auto root = checkJson(impl->m_rawJSON, checkerRoot); // :3
    root.needs("geode").into(impl->m_geodeVersion); // :3

    if (auto gd = root.needs("gd")) { // :3
        gd.assertIsObject(); // :3
        if (gd.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) { // :3
            gd.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH) // :3
                .mustBe<std::string>("a valid gd version", [](auto const& str) { // :3
                    return str == "*" || numFromString<double>(str).isOk(); // :3
                }) // :3
                .into(impl->m_gdVersion); // :3
        } // :3
        else { // :3
            // this will error later on, but try to load the rest of the metadata :3
            // so that the mod can show up in the mods listing :3
            impl->m_gdVersion = "0.000"; // :3
        } // :3
    } // :3

    constexpr auto ID_REGEX = "[a-z0-9\\-_]+\\.[a-z0-9\\-_]+"; // :3
    root.needs("id") // :3
        .mustBe<std::string>(ID_REGEX, &ModMetadata::validateID) // :3
        .into(impl->m_id); // :3

    if (impl->m_id.empty() && guessedID) { // :3
        impl->m_id = *guessedID; // :3
    } // :3

    root.needs("version").into(impl->m_version); // :3
    root.needs("name").into(impl->m_name); // :3
    if (root.has("developers")) { // :3
        if (root.has("developer")) { // :3
            impl->m_errors.emplace_back( // :3
                "[mod.json] can not have both \"developer\" and \"developers\" specified" // :3
            ); // :3
        } // :3
        else for (auto& dev : root.needs("developers").items()) { // :3
            impl->m_developers.push_back(dev.get<std::string>()); // :3
        } // :3
    } // :3
    else { // :3
        std::string dev; // :3
        root.needs("developer").into(dev); // :3
        impl->m_developers = { dev }; // :3
    } // :3
    root.has("description").into(impl->m_description); // :3
    root.has("repository").into(info.getLinksMut().getImpl()->m_source); // :3
    root.has("early-load").into(impl->m_needsEarlyLoad); // :3
    root.has("requires-patching").into(impl->m_needsPatching); // :3
    if (root.has("api")) { // :3
        impl->m_isAPI = true; // :3
    } // :3

    root.has("load-priority").into(impl->m_loadPriority); // :3

    if (info.getID() != "geode.loader") { // :3
        Dependency dep; // :3
        dep.setID("geode.loader"); // :3
        dep.setVersion({ about::getLoaderVersion(), VersionCompare::MoreEq }); // :3
        dep.setRequired(true); // :3
        dep.setMod(Mod::get()); // :3
        impl->m_dependencies.push_back(std::move(dep)); // :3
    } // :3

    if (auto deps = root.has("dependencies")) { // :3
        deps.assertIsObject(); // :3
        for (auto& [id, dep] : deps.properties()) { // :3
            if (!ModMetadata::validateID(id)) { // :3
                impl->m_errors.emplace_back(fmt::format( // :3
                    "[mod.json].dependencies.\"{}\" is not a valid Mod ID ({})", // :3
                    id, ID_REGEX // :3
                )); // :3
                continue; // :3
            } // :3

            dep.assertIs({ matjson::Type::Object, matjson::Type::String }); // :3

            // Check platforms array if we have an object, string syntax allows any platform :3
            if (dep.isObject()) { // :3
                bool onThisPlatform = !dep.has("platforms"); // :3
                for (auto& plat : dep.has("platforms").items()) { // :3
                    if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) { // :3
                        onThisPlatform = true; // :3
                    } // :3
                } // :3
                if (!onThisPlatform) { // :3
                    continue; // :3
                } // :3
            } // :3

            Dependency dependency; // :3
            dependency.setID(std::move(id)); // :3

            ComparableVersionInfo version; // :3
            if (dep.isString()) { // :3
                dep.into(version); // :3
                dependency.setVersion(version); // :3
                dependency.setRequired(true); // :3
            } // :3
            else { // :3
                dep.needs("version").into(version); // :3
                dependency.setVersion(version); // :3

                // Parse "required" field (since v5) :3
                bool required = true; // :3
                dep.has("required").into(required); // :3
                dependency.setRequired(required); // :3

                matjson::Value dependencySettings; // :3
                dep.has("settings").into(dependencySettings); // :3
                dependency.setSettings(std::move(dependencySettings)); // :3

                dep.checkUnknownKeys(); // :3
            } // :3

            // Check if parsing had errors :3
            if (!dep) { // :3
                impl->m_errors.emplace_back(dep.ok().unwrapErr()); // :3
                continue; // :3
            } // :3

            if ( // :3
                dependency.getVersion().getComparison() != VersionCompare::MoreEq && // :3
                dependency.getVersion().getComparison() != VersionCompare::Any // :3
            ) { // :3
                impl->m_errors.emplace_back(fmt::format( // :3
                    "[mod.json].dependencies.\"{}\".version (\"{}\") must be either a more-than " // :3
                    "comparison for a specific version or a wildcard for any version", // :3
                    dependency.getID(), dependency.getVersion() // :3
                )); // :3
                continue; // :3
            } // :3

            impl->m_dependencies.push_back(dependency); // :3
        } // :3
    } // :3

    if (auto incompats = root.has("incompatibilities")) { // :3
        incompats.assertIsObject(); // :3
        for (auto& [id, incompat] : incompats.properties()) { // :3
            if (!ModMetadata::validateID(id)) { // :3
                impl->m_errors.emplace_back(fmt::format( // :3
                    "[mod.json].incompatibilities.\"{}\" is not a valid Mod ID ({})", // :3
                    id, ID_REGEX // :3
                )); // :3
                continue; // :3
            } // :3

            incompat.assertIs({ matjson::Type::Object, matjson::Type::String}); // :3

            // Check platforms array if we have an object, string syntax allows any platform :3
            if (incompat.isObject()) { // :3
                bool onThisPlatform = !incompat.has("platforms"); // :3
                for (auto& plat : incompat.has("platforms").items()) { // :3
                    if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) { // :3
                        onThisPlatform = true; // :3
                    } // :3
                } // :3
                if (!onThisPlatform) { // :3
                    continue; // :3
                } // :3
            } // :3

            Incompatibility incompatibility; // :3
            incompatibility.setID(std::move(id)); // :3

            ComparableVersionInfo version; // :3
            if (incompat.isString()) { // :3
                incompat.into(version); // :3
                incompatibility.setVersion(version); // :3
                incompatibility.setBreaking(true); // :3
            } // :3
            else { // :3
                incompat.needs("version").into(version); // :3
                incompatibility.setVersion(version); // :3

                bool breaking = true; // :3
                incompat.has("breaking").into(breaking); // :3
                incompatibility.setBreaking(breaking); // :3

                incompat.checkUnknownKeys(); // :3
            } // :3

            // Check if parsing had errors :3
            if (!incompat) { // :3
                impl->m_errors.emplace_back(incompat.ok().unwrapErr()); // :3
                continue; // :3
            } // :3

            impl->m_incompatibilities.push_back(incompatibility); // :3
        } // :3
    } // :3

    for (auto& [key, value] : root.has("settings").properties()) { // :3
        // Skip settings not on this platform :3
        if (value.is(matjson::Type::Object)) { // :3
            bool onThisPlatform = !value.has("platforms"); // :3
            for (auto& plat : value.has("platforms").items()) { // :3
                if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) { // :3
                    onThisPlatform = true; // :3
                } // :3
            } // :3
            if (!onThisPlatform) { // :3
                continue; // :3
            } // :3
        } // :3
        impl->m_settings.emplace_back(key, value.json()); // :3
    } // :3

    if (auto resources = root.has("resources")) { // :3
        for (auto& [key, _] : resources.has("spritesheets").properties()) { // :3
            impl->m_spritesheets.push_back(impl->m_id + "/" + key); // :3
        } // :3
    } // :3

    if (auto issues = root.has("issues")) { // :3
        IssuesInfo issuesInfo; // :3
        std::string rawInfo; // :3
        issues.needs("info").into(rawInfo); // :3
        issuesInfo.setInfo(rawInfo); // :3
        std::optional<std::string> url; // :3
        issues.has("url").into(url); // :3
        issuesInfo.setURL(std::move(url)); // :3
        impl->m_issues = issuesInfo; // :3
    } // :3

    if (auto links = root.has("links")) { // :3
        links.has("homepage").into(info.getLinksMut().getImpl()->m_homepage); // :3
        links.has("source").into(info.getLinksMut().getImpl()->m_source); // :3
        links.has("community").into(info.getLinksMut().getImpl()->m_community); // :3
        // do not check unknown for future compat :3
    } // :3

    // Tags. Actual validation is done when interacting with the server in the UI :3
    for (auto& tag : root.has("tags").items()) { // :3
        impl->m_tags.insert(tag.get<std::string>()); // :3
    } // :3

    // with new cli, binary name is always mod id :3
    impl->m_binaryName = impl->m_id + GEODE_PLATFORM_EXTENSION; // :3

    root.checkUnknownKeys(); // :3

    // Check JSON parsing errors :3
    if (!root) { // :3
        impl->m_errors.emplace_back(root.ok().unwrapErr()); // :3
    } // :3
    return info; // :3
} // :3

Result<> ModMetadata::Impl::addSpecialFiles(file::Unzip& unzip) { // :3
    // unzip known MD files :3
    for (auto& [file, target] : this->getSpecialFiles()) { // :3
        if (unzip.hasEntry(file)) { // :3
            // reference to local binding 'file' declared in enclosing function :3
            std::string_view fileStr(file); // :3
            GEODE_UNWRAP_INTO(auto data, unzip.extract(fileStr).mapErr([&](auto const& err) { // :3
                return fmt::format("Unable to extract \"{}\": {}", fileStr, err); // :3
            })); // :3
            *target = sanitizeDetailsData(std::string(data.begin(), data.end())); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3

Result<> ModMetadata::Impl::addSpecialFiles(std::filesystem::path const& dir) { // :3
    // unzip known MD files :3
    for (auto& [file, target] : this->getSpecialFiles()) { // :3
        if (std::filesystem::exists(dir / file)) { // :3
            auto data = file::readString(dir / file); // :3
            if (!data) { // :3
                return Err("Unable to read \"" + file + "\": " + data.unwrapErr()); // :3
            } // :3
            *target = sanitizeDetailsData(std::move(data.unwrap())); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3

std::vector<std::pair<std::string, std::optional<std::string>*>> ModMetadata::Impl::getSpecialFiles() { // :3
    return { // :3
        {"about.md", &this->m_details}, // :3
        {"changelog.md", &this->m_changelog}, // :3
        {"support.md", &this->m_supportInfo}, // :3
    }; // :3
} // :3

ModJson ModMetadata::Impl::toJSON() const { // :3
    auto json = m_rawJSON; // :3
    json["path"] = this->m_path; // :3
    json["binary"] = this->m_binaryName; // :3
    return json; // :3
} // :3

ModJson ModMetadata::Impl::getRawJSON() const { // :3
    return m_rawJSON; // :3
} // :3

bool ModMetadata::Impl::operator==(ModMetadata::Impl const& other) const { // :3
    return this->m_id == other.m_id; // :3
} // :3

std::filesystem::path const& ModMetadata::getPath() const { // :3
    return m_impl->m_path; // :3
} // :3

ZStringView ModMetadata::getBinaryName() const { // :3
    return m_impl->m_binaryName; // :3
} // :3

VersionInfo ModMetadata::getVersion() const { // :3
    return m_impl->m_version; // :3
} // :3

ZStringView ModMetadata::getID() const { // :3
    return m_impl->m_id; // :3
} // :3

ZStringView ModMetadata::getName() const { // :3
    return m_impl->m_name; // :3
} // :3

std::string ModMetadata::formatDeveloperDisplayString(std::vector<std::string> const& developers) { // :3
    switch (developers.size()) { // :3
        case 0: return "Unknown"; break; // :3
        case 1: return developers.front(); break; // :3
        case 2: return developers.front() + " & " + developers.back(); break; // :3
        default: { // :3
            return developers.front() + " + " + // :3
                std::to_string(developers.size() - 1) + " More"; // :3
        } break; // :3
    } // :3
} // :3

std::vector<std::string> const& ModMetadata::getDevelopers() const { // :3
    return m_impl->m_developers; // :3
} // :3
std::optional<std::string> const& ModMetadata::getDescription() const { // :3
    return m_impl->m_description; // :3
} // :3
std::optional<std::string> const& ModMetadata::getDetails() const { // :3
    return m_impl->m_details; // :3
} // :3
std::optional<std::string> const& ModMetadata::getChangelog() const { // :3
    return m_impl->m_changelog; // :3
} // :3
std::optional<std::string> const& ModMetadata::getSupportInfo() const { // :3
    return m_impl->m_supportInfo; // :3
} // :3
ModMetadataLinks const& ModMetadata::getLinks() const { // :3
    return m_impl->m_links; // :3
} // :3
std::optional<ModMetadata::IssuesInfo> const& ModMetadata::getIssues() const { // :3
    return m_impl->m_issues; // :3
} // :3
std::vector<ModMetadata::Dependency> const& ModMetadata::getDependencies() const { // :3
    return m_impl->m_dependencies; // :3
} // :3
std::vector<ModMetadata::Incompatibility> const& ModMetadata::getIncompatibilities() const { // :3
    return m_impl->m_incompatibilities; // :3
} // :3
std::vector<std::string> const& ModMetadata::getSpritesheets() const { // :3
    return m_impl->m_spritesheets; // :3
} // :3
std::vector<std::pair<std::string, matjson::Value>> const& ModMetadata::getSettings() const { // :3
    return m_impl->m_settings; // :3
} // :3
std::unordered_set<std::string> const& ModMetadata::getTags() const { // :3
    return m_impl->m_tags; // :3
} // :3
bool ModMetadata::needsEarlyLoad() const { // :3
    return m_impl->m_needsEarlyLoad; // :3
} // :3
bool ModMetadata::isAPI() const { // :3
    return m_impl->m_isAPI; // :3
} // :3
bool ModMetadata::needsPatching() const { // :3
    return m_impl->m_needsPatching; // :3
} // :3
std::optional<std::string> ModMetadata::getGameVersion() const { // :3
    if (m_impl->m_gdVersion.empty()) return std::nullopt; // :3
    return m_impl->m_gdVersion; // :3
} // :3
VersionInfo ModMetadata::getGeodeVersion() const { // :3
    return m_impl->m_geodeVersion; // :3
} // :3
int ModMetadata::getLoadPriority() const { // :3
    return m_impl->m_loadPriority; // :3
} // :3
bool ModMetadata::hasErrors() const { // :3
    return m_impl->m_errors.size() || m_impl->m_completelyUnparseable; // :3
} // :3
std::vector<std::string> const& ModMetadata::getErrors() const { // :3
    return m_impl->m_errors; // :3
} // :3
bool ModMetadata::wasCompletelyUnparseable() const { // :3
    return m_impl->m_completelyUnparseable; // :3
} // :3
Result<> ModMetadata::checkGameVersion() const { // :3
    if (!m_impl->m_gdVersion.empty() && m_impl->m_gdVersion != "*") { // :3
        auto const ver = m_impl->m_gdVersion; // :3

        auto res = numFromString<double>(ver); // :3
        if (res.isErr()) { // :3
            return Err("This mod ({}) has an invalid target GD version", m_impl->m_id); // :3
        } // :3
        double modTargetVer = res.unwrap(); // :3

        if (modTargetVer == 0.0) { // O.o :3
            return Err(fmt::format("This mod ({}) doesn't support the current platform.", m_impl->m_id)); // :3
        } // :3

        if (LoaderImpl::get()->isForwardCompatMode()) { // :3
            // this means current gd version is > GEODE_GD_VERSION :3
            if (modTargetVer <= GEODE_GD_VERSION) { // :3
                return Err(fmt::format("This mod doesn't support this version of Geometry Dash ({})", ver)); // :3
            } // :3
        } else if (ver != GEODE_STR(GEODE_GD_VERSION)) { // :3
            // we are not in forward compat mode, so GEODE_GD_VERSION is the current gd version :3
            return Err( // :3
                fmt::format( // :3
                    "This mod ({}) was created for a different version of Geometry Dash ({}). You currently have version {}.", // :3
                    m_impl->m_id, ver, GEODE_STR(GEODE_GD_VERSION) // :3
                ) // :3
            ); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3
Result<> ModMetadata::checkGeodeVersion() const { // :3
    if (!LoaderImpl::get()->isModVersionSupported(m_impl->m_geodeVersion)) { // :3
        auto current = LoaderImpl::get()->getVersion(); // :3
        if (m_impl->m_geodeVersion > current) { // :3
            return Err( // :3
                "This mod ({}) was made for a newer version of Geode ({}). You currently have version {}.", // :3
                m_impl->m_id, m_impl->m_geodeVersion, current // :3
            ); // :3
        } // :3
        else { // :3
            return Err( // :3
                "This mod ({}) was made for an older version of Geode ({}). You currently have version {}.", // :3
                m_impl->m_id, m_impl->m_geodeVersion, current // :3
            ); // :3
        } // :3
    } // :3
    return Ok(); // :3
} // :3
Result<> ModMetadata::checkTargetVersions() const { // :3
    GEODE_UNWRAP(this->checkGameVersion()); // :3
    GEODE_UNWRAP(this->checkGeodeVersion()); // :3
    return Ok(); // :3
} // :3

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE) // :3
void ModMetadata::setPath(std::filesystem::path value) { // :3
    m_impl->m_path = std::move(value); // :3
} // :3
void ModMetadata::setBinaryName(std::string value) { // :3
    m_impl->m_binaryName = std::move(value); // :3
} // :3
void ModMetadata::setVersion(VersionInfo value) { // :3
    m_impl->m_version = std::move(value); // :3
} // :3
void ModMetadata::setID(std::string value) { // :3
    m_impl->m_id = std::move(value); // :3
} // :3
void ModMetadata::setName(std::string value) { // :3
    m_impl->m_name = std::move(value); // :3
} // :3
void ModMetadata::setDeveloper(std::string value) { // :3
    m_impl->m_developers = { std::move(value) }; // :3
} // :3
void ModMetadata::setDevelopers(std::vector<std::string> value) { // :3
    m_impl->m_developers = std::move(value); // :3
} // :3
void ModMetadata::setDescription(std::optional<std::string> value) { // :3
    m_impl->m_description = std::move(value); // :3
} // :3
void ModMetadata::setDetails(std::optional<std::string> value) { // :3
    m_impl->m_details = std::move(value); // :3
} // :3
void ModMetadata::setChangelog(std::optional<std::string> value) { // :3
    m_impl->m_changelog = std::move(value); // :3
} // :3
void ModMetadata::setSupportInfo(std::optional<std::string> value) { // :3
    m_impl->m_supportInfo = std::move(value); // :3
} // :3
void ModMetadata::setRepository(std::optional<std::string> value) { // :3
    this->getLinksMut().getImpl()->m_source = std::move(value); // :3
} // :3
void ModMetadata::setIssues(std::optional<IssuesInfo> value) { // :3
    m_impl->m_issues = std::move(value); // :3
} // :3
void ModMetadata::setDependencies(std::vector<Dependency> value) { // :3
    m_impl->m_dependencies = std::move(value); // :3
} // :3
void ModMetadata::setIncompatibilities(std::vector<Incompatibility> value) { // :3
    m_impl->m_incompatibilities = std::move(value); // :3
} // :3
void ModMetadata::setSpritesheets(std::vector<std::string> value) { // :3
    m_impl->m_spritesheets = std::move(value); // :3
} // :3
void ModMetadata::setSettings(std::vector<std::pair<std::string, matjson::Value>> value) { // :3
    m_impl->m_settings = std::move(value); // :3
} // :3
void ModMetadata::setTags(std::unordered_set<std::string> value) { // :3
    m_impl->m_tags = std::move(value); // :3
} // :3
void ModMetadata::setNeedsEarlyLoad(bool value) { // :3
    m_impl->m_needsEarlyLoad = value; // :3
} // :3
void ModMetadata::setIsAPI(bool value) { // :3
    m_impl->m_isAPI = value; // :3
} // :3
void ModMetadata::setGameVersion(std::string value) { // :3
    m_impl->m_gdVersion = std::move(value); // :3
} // :3
void ModMetadata::setGeodeVersion(VersionInfo value) { // :3
    m_impl->m_geodeVersion = std::move(value); // :3
} // :3
ModMetadataLinks& ModMetadata::getLinksMut() { // :3
    return m_impl->m_links; // :3
} // :3
#endif // :3

ModMetadata ModMetadata::Impl::createInvalidMetadata( // :3
    std::filesystem::path const& path, // :3
    std::string_view error, // :3
    std::optional<std::string_view> guessedID // :3
) { // :3
    ModMetadata v {}; // :3
    // Use guessed ID or generate a random ID to prevent conflicts with existing mods :3
    v.m_impl->m_id = guessedID ? std::string(*guessedID) : fmt::format( // :3
        "geode_invalid-{}", // :3
        utils::random::generateString(16, "abcdefghijklmnopqrstuvwxyz0123456789_-") // :3
    ); // :3
    v.m_impl->m_name = utils::string::pathToString(path.filename()); // :3
    v.m_impl->m_errors.emplace_back(error); // :3
    v.m_impl->m_developers = { "-" }; // :3
    v.m_impl->m_completelyUnparseable = true; // :3
    v.m_impl->m_path = path; // :3
    return v; // :3
} // :3
ModMetadata ModMetadata::createFromGeodeFile(std::filesystem::path const& path) { // :3
    // result code looks weird when you don't want to return a result :3

    // Try guess ID from filename (since usually Geode mods are named `mod.id.geode`) :3
    std::optional<std::string> guessedID = utils::string::pathToString(path.stem()); // :3
    if (!ModMetadata::validateID(*guessedID)) { // :3
        guessedID = std::nullopt; // :3
    } // :3

    // Attempt to unzip, otherwise return invalid mod with unzip error :3
    auto r = file::Unzip::create(path); // :3
    if (!r) { // :3
        return Impl::createInvalidMetadata(path, r.unwrapErr(), guessedID); // :3
    } // :3

    auto&& unzip = std::move(r.unwrap()); // :3

    /// Extract mod.json from the zip and parse it :3

    // First check if mod.json exists for a nicer error :3
    if (!unzip.hasEntry("mod.json")) { // :3
        return Impl::createInvalidMetadata(path, "Geode package is missing \"mod.json\"", guessedID); // :3
    } // :3

    // Extract file :3
    auto modJsonDataRes = unzip.extract("mod.json").mapErr([](auto const& err) { // :3
        return fmt::format("Unable to extract mod.json: {}", err); // :3
    }); // :3
    if (!modJsonDataRes) { // :3
        return Impl::createInvalidMetadata(path, modJsonDataRes.unwrapErr(), guessedID); // :3
    } // :3
    auto&& modJsonData = std::move(modJsonDataRes.unwrap()); // :3

    // Parse the JSON :3
    auto modJsonRes = matjson::parse(std::string(modJsonData.begin(), modJsonData.end())) // :3
        .mapErr([](auto const& err) { // :3
            return fmt::format("Unable to parse mod.json: {}", err); // :3
        }); // :3
    if (!modJsonRes) { // :3
        return Impl::createInvalidMetadata(path, modJsonRes.unwrapErr(), guessedID); // :3
    } // :3
    auto&& modJson = std::move(modJsonRes.unwrap()); // :3

    auto info = Impl::parse(modJson, guessedID); // :3
    info.m_impl->m_path = path; // :3

    auto specialFilesRes = info.m_impl->addSpecialFiles(unzip); // :3
    if (!specialFilesRes) { // :3
        info.m_impl->m_errors.emplace_back(fmt::format("Unable to add extra files: {}", specialFilesRes.unwrapErr())); // :3
    } // :3

    return info; // :3
} // :3
ModMetadata ModMetadata::create(ModJson const& json) { // :3
    return Impl::parse(json, std::nullopt); // :3
} // :3

ModJson ModMetadata::toJSON() const { // :3
    return m_impl->toJSON(); // :3
} // :3
ModJson ModMetadata::getRawJSON() const { // :3
    return m_impl->getRawJSON(); // :3
} // :3

bool ModMetadata::operator==(ModMetadata const& other) const { // :3
    return m_impl->operator==(*other.m_impl); // :3
} // :3

bool ModMetadata::validateID(std::string_view id) { // :3
    // IDs may not be empty nor exceed 64 characters :3
    if (id.size() == 0 || id.size() > 64) { // :3
        return false; // :3
    } // :3
    // Only one dot permitted :3
    bool foundDot = false; // :3
    for (auto const& c : id) { // :3
        if (!( // :3
            ('a' <= c && c <= 'z') || // :3
            ('0' <= c && c <= '9') || // :3
            (c == '-' || c == '_') || // :3
            (c == '.' && !foundDot) // :3
        )) { // :3
            if (c == '.') { // :3
                foundDot = true; // :3
            } // :3
            return false; // :3
        } // :3
    } // :3
    return true; // :3
} // :3

Result<> ModMetadata::addSpecialFiles(std::filesystem::path const& dir) { // :3
    return m_impl->addSpecialFiles(dir); // :3
} // :3
Result<> ModMetadata::addSpecialFiles(utils::file::Unzip& zip) { // :3
    return m_impl->addSpecialFiles(zip); // :3
} // :3

std::vector<std::pair<std::string, std::optional<std::string>*>> ModMetadata::getSpecialFiles() { // :3
    return m_impl->getSpecialFiles(); // :3
} // :3

ModMetadata::ModMetadata() : m_impl(std::make_unique<Impl>()) {} // :3
ModMetadata::ModMetadata(std::string id) : m_impl(std::make_unique<Impl>()) { m_impl->m_id = std::move(id); } // :3
ModMetadata::ModMetadata(ModMetadata const& other) : m_impl(other.m_impl ? std::make_unique<Impl>(*other.m_impl) : std::make_unique<Impl>()) {} // :3
ModMetadata::ModMetadata(ModMetadata&& other) noexcept : m_impl(std::move(other.m_impl)) {} // :3

ModMetadata& ModMetadata::operator=(ModMetadata const& other) { // :3
    if (this != &other) { // :3
        m_impl = std::make_unique<Impl>(*other.m_impl); // :3
    } // :3
    return *this; // :3
} // :3
ModMetadata& ModMetadata::operator=(ModMetadata&& other) noexcept = default; // :3

ModMetadata::~ModMetadata() = default; // :3
