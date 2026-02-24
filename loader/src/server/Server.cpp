#include "Server.hpp" // :3
#include <Geode/utils/JsonValidation.hpp> // :3
#include <Geode/utils/ranges.hpp> // :3
#include <chrono> // :3
#include <fmt/core.h> // :3
#include <loader/ModMetadataImpl.hpp> // :3
#include <fmt/chrono.h> // :3
#include <arc/sync/Mutex.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include "../internal/about.hpp" // :3
#include "Geode/loader/Loader.hpp" // :3

using namespace server; // :3

#define GEODE_GD_VERSION_STR GEODE_STR(GEODE_GD_VERSION) // :3

template <class K, class V> // :3
    requires std::equality_comparable<K> && std::copy_constructible<K> // :3
class CacheMap final { // :3
private: // :3
    // I know this looks like a goofy choice over just :3
    // `std::unordered_map`, but hear me out: :3
    // :3
    // This needs preserved insertion order (so shrinking the cache :3
    // to match size limits doesn't just have to erase random :3
    // elements) :3
    // :3
    // If this used a map for values and another vector for storing :3
    // insertion order, it would have a pretty big memory footprint :3
    // (two copies of Query, one for order, one for map + two heap :3
    // allocations on top of that) :3
    // :3
    // In addition, it would be a bad idea to have a cache of 1000s :3
    // of items in any case (since that would likely take up a ton :3
    // of memory, which we want to avoid since it's likely many :3
    // crashes with the old index were due to too much memory :3
    // usage) :3
    // :3
    // Linear searching a vector of at most a couple dozen items is :3
    // lightning-fast (🚀), and besides the main performance benefit :3
    // comes from the lack of a web request - not how many extra :3
    // milliseconds we can squeeze out of a map access :3
    std::vector<std::pair<K, V>> m_values; // :3
    size_t m_sizeLimit = 20; // :3

public: // :3
    std::optional<V> get(K const& key) { // :3
        auto it = std::find_if(m_values.begin(), m_values.end(), [&key](auto const& q) { // :3
            return q.first == key; // :3
        }); // :3
        if (it != m_values.end()) { // :3
            return it->second; // :3
        } // :3
        return std::nullopt; // :3
    } // :3
    bool has(K const& key) { // :3
        return std::find_if(m_values.begin(), m_values.end(), [&key](auto const& q) { // :3
            return q.first == key; // :3
        }) != m_values.end(); // :3
    } // :3
    void add(K&& key, V&& value) { // :3
        auto pair = std::make_pair(std::move(key), std::move(value)); // :3

        // Shift and replace last element if we're at cache size limit :3
        if (m_values.size() >= m_sizeLimit) { // :3
            std::shift_left(m_values.begin(), m_values.end(), 1); // :3
            m_values.back() = std::move(pair); // :3
        } // :3
        // Otherwise append at end :3
        else { // :3
            m_values.emplace_back(std::move(pair)); // :3
        } // :3
    } // :3
    void remove(K const& key) { // :3
        ranges::remove(m_values, [&key](auto const& q) { return q.first == key; }); // :3
    } // :3
    void clear() { // :3
        m_values.clear(); // :3
    } // :3
    void limit(size_t size) { // :3
        m_sizeLimit = size; // :3
        m_values.clear(); // :3
    } // :3
    size_t size() const { // :3
        return m_values.size(); // :3
    } // :3
    size_t limit() const { // :3
        return m_sizeLimit; // :3
    } // :3
}; // :3

template <class F> // :3
struct ExtractFun; // :3

template <class V, class... Args> // :3
struct ExtractFun<ServerFuture<V>(*)(Args...)> { // :3
    using CacheKey = std::tuple<std::remove_cvref_t<Args>...>; // :3
    using Value = V; // :3

    template <class... CArgs> // :3
    static CacheKey key(CArgs&&... args) { // :3
        return std::make_tuple(std::forward<CArgs>(args)..., false); // :3
    } // :3
    template <class... CArgs> // :3
    static ServerFuture<V> invoke(auto&& func, CArgs const&... args) { // :3
        return func(args..., false); // :3
    } // :3
}; // :3

template <auto F> // :3
class FunCache final { // :3
public: // :3
    using Extract  = ExtractFun<decltype(F)>; // :3
    using CacheKey = typename Extract::CacheKey; // :3
    using Value    = typename Extract::Value; // :3

private: // :3
    asp::Mutex<CacheMap<CacheKey, Value>> m_cache; // :3

public: // :3
    FunCache() = default; // :3
    FunCache(FunCache const&) = delete; // :3
    FunCache(FunCache&&) = delete; // :3

    template <class... Args> // :3
    arc::Future<Result<Value, ServerError>> get(Args&&... args) { // :3
        ARC_FRAME(); // :3
        auto key = Extract::key(args...); // :3

        auto cache = m_cache.lock(); // :3
        if (auto v = cache->get(key)) { // :3
            co_return Ok(std::move(*v)); // :3
        } // :3
        cache.unlock(); // :3

        auto f = ARC_CO_UNWRAP(co_await Extract::invoke(F, std::forward<Args>(args)...)); // :3

        cache.relock(); // :3
        if (cache->has(key)) { // :3
            co_return Ok(std::move(f)); // don't save to cache if someone beat us :3
        } // :3
        cache->add(std::move(key), Value{f}); // :3

        co_return Ok(f); // :3
    } // :3

    template <class... Args> // :3
    void remove(Args const&... args) { // :3
        m_cache.lock()->remove(Extract::key(args...)); // :3
    } // :3

    size_t size() { // :3
        return m_cache.lock()->size(); // :3
    } // :3
    void limit(size_t size) { // :3
        m_cache.lock()->limit(size); // :3
    } // :3
    void clear() { // :3
        m_cache.lock()->clear(); // :3
    } // :3
}; // :3

template <auto F> // :3
FunCache<F>& getCache() { // :3
    static auto inst = FunCache<F>(); // :3
    return inst; // :3
} // :3

static const char* jsonTypeToString(matjson::Type const& type) { // :3
    switch (type) { // :3
        case matjson::Type::Object: return "object"; // :3
        case matjson::Type::Array: return "array"; // :3
        case matjson::Type::Bool: return "boolean"; // :3
        case matjson::Type::Number: return "number"; // :3
        case matjson::Type::String: return "string"; // :3
        case matjson::Type::Null: return "null"; // :3
        default: return "unknown"; // :3
    } // :3
} // :3

static Result<matjson::Value, ServerError> parseServerPayload(web::WebResponse const& response) { // :3
    auto asJson = response.json(); // :3
    if (!asJson) { // :3
        return Err(ServerError(response.code(), "Response was not valid JSON: {}", asJson.unwrapErr())); // :3
    } // :3
    auto json = std::move(asJson).unwrap(); // :3
    if (!json.isObject()) { // :3
        return Err(ServerError(response.code(), "Expected object, got {}", jsonTypeToString(json.type()))); // :3
    } // :3
    if (!json.contains("payload")) { // :3
        return Err(ServerError(response.code(), "Object does not contain \"payload\" key - got {}", json.dump())); // :3
    } // :3
    return Ok(std::move(json["payload"])); // :3
} // :3

static ServerError parseServerError(web::WebResponse const& error) { // :3
    // The server should return errors as `{ "error": "...", "payload": "" }` :3
    if (auto asJson = error.json()) { // :3
        auto& json = asJson.unwrap(); // :3
        if (json.isObject() && json.contains("error") && json["error"].isString()) { // :3
            return ServerError( // :3
                error.code(), // :3
                "{}", json["error"].asString().unwrapOr("Unknown (no error message)") // :3
            ); // :3
        } // :3
        else { // :3
            return ServerError(error.code(), "Unknown (not valid JSON)"); // :3
        } // :3
    } // :3
    // But if we get something else for some reason, return that :3
    else { // :3
        return ServerError( // :3
            error.code(), // :3
            "{}", error.string().unwrapOr("Unknown (not a valid string)") // :3
        ); // :3
    } // :3
} // :3

static ServerProgress parseServerProgress(web::WebProgress const& prog, auto msg) { // :3
    if (auto per = prog.downloadProgress()) { // :3
        return ServerProgress(msg, static_cast<uint8_t>(*per)); // :3
    } // :3
    else { // :3
        return ServerProgress(msg); // :3
    } // :3
} // :3

const char* server::sortToString(ModsSort sorting) { // :3
    switch (sorting) { // :3
        default: // :3
        case ModsSort::Downloads: return "downloads"; // :3
        case ModsSort::RecentlyUpdated: return "recently_updated"; // :3
        case ModsSort::RecentlyPublished: return "recently_published"; // :3
    } // :3
} // :3

Result<ServerTag> ServerTag::parse(matjson::Value raw) { // :3
    auto root = checkJson(std::move(raw), "ServerTag"); // :3
    auto res = ServerTag(); // :3

    root.needs("id").into(res.id); // :3
    root.needs("name").into(res.name); // :3
    root.needs("display_name").into(res.displayName); // :3

    return root.ok(std::move(res)); // :3
} // :3
Result<std::vector<ServerTag>> ServerTag::parseList(matjson::Value raw) { // :3
    auto payload = checkJson(std::move(raw), "ServerTagsList"); // :3
    std::vector<ServerTag> list {}; // :3
    for (auto& item : payload.items()) { // :3
        auto mod = ServerTag::parse(item.takeJson()); // :3
        if (mod) { // :3
            list.push_back(std::move(mod).unwrap()); // :3
        } // :3
        else { // :3
            log::error("Unable to parse tag from the server: {}", mod.unwrapErr()); // :3
        } // :3
    } // :3
    return payload.ok(std::move(list)); // :3
} // :3

Result<ServerDateTime> ServerDateTime::parse(std::string const& str) { // :3
    #ifdef GEODE_IS_WINDOWS // :3
    std::stringstream ss(str); // :3
    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> seconds; // :3
    if (ss >> std::chrono::parse("%Y-%m-%dT%H:%M:%S%Z", seconds)) { // :3
        return Ok(ServerDateTime { // :3
            .value = seconds // :3
        }); // :3
    } // :3
    return Err("Invalid date time format '{}'", str); // :3
    #else // :3
    tm t; // :3
    auto ptr = strptime(str.c_str(), "%Y-%m-%dT%H:%M:%SZ", &t); // :3
    if (ptr != str.data() + str.size()) { // :3
        return Err("Invalid date time format '{}'", str); // :3
    } // :3
    auto time = timegm(&t); // :3
    return Ok(ServerDateTime { // :3
        .value = std::chrono::system_clock::from_time_t(time) // :3
    }); // :3
    #endif // :3
} // :3

Result<ServerModVersion> ServerModVersion::parse(matjson::Value raw) { // :3
    auto root = checkJson(std::move(raw), "ServerModVersion"); // :3

    auto res = ServerModVersion(); // :3

    res.metadata.setGeodeVersion(root.needs("geode").get<VersionInfo>()); // :3

    // Verify target GD version :3
    auto gd_obj = root.needs("gd"); // :3
    std::string gd = "0.000"; // :3
    if (gd_obj.hasNullable(GEODE_PLATFORM_SHORT_IDENTIFIER)) { // :3
        gd = gd_obj.hasNullable(GEODE_PLATFORM_SHORT_IDENTIFIER). get<std::string>(); // :3
    } // :3

    if (gd != "*") { // :3
        res.metadata.setGameVersion(gd); // :3
    } // :3

    // Get server info :3
    root.needs("download_link").into(res.downloadURL); // :3
    root.needs("download_count").into(res.downloadCount); // :3
    root.needs("hash").into(res.hash); // :3

    // Get mod metadata info :3
    res.metadata.setID(root.needs("mod_id").get<std::string>()); // :3
    res.metadata.setName(root.needs("name").get<std::string>()); // :3
    res.metadata.setDescription(root.needs("description").get<std::string>()); // :3
    res.metadata.setVersion(root.needs("version").get<VersionInfo>()); // :3
    res.metadata.setIsAPI(root.needs("api").get<bool>()); // :3

    std::vector<ModMetadata::Dependency> dependencies {}; // :3
    for (auto& obj : root.hasNullable("dependencies").items()) { // :3
        // todo: this should probably be generalized to use the same function as mod.json :3

        bool onThisPlatform = !obj.hasNullable("platforms"); // :3
        for (auto& plat : obj.hasNullable("platforms").items()) { // :3
            if (PlatformID::coveredBy(plat.get<std::string>(), GEODE_PLATFORM_TARGET)) { // :3
                onThisPlatform = true; // :3
            } // :3
        } // :3
        if (!onThisPlatform) { // :3
            continue; // :3
        } // :3

        ModMetadata::Dependency dependency; // :3
        std::string modId; // :3
        obj.needs("mod_id").mustBe<std::string>("a valid id", &ModMetadata::validateID).into(modId); // :3
        dependency.setID(modId); // :3
        ComparableVersionInfo version; // :3
        obj.needs("version").into(version); // :3
        dependency.setVersion(version); // :3
        bool required; // :3
        obj.hasNullable("required").into(required); // :3
        dependency.setRequired(required); // :3

        // Check if this dependency is installed, and if so assign the `mod` member to mark that :3
        auto mod = Loader::get()->getInstalledMod(dependency.getID()); // :3
        if (mod && dependency.getVersion().compare(mod->getVersion())) { // :3
            dependency.setMod(mod); // :3
        } // :3

        dependencies.push_back(std::move(dependency)); // :3
    } // :3
    res.metadata.setDependencies(std::move(dependencies)); // :3

    std::vector<ModMetadata::Incompatibility> incompatibilities {}; // :3
    for (auto& obj : root.hasNullable("incompatibilities").items()) { // :3
        ModMetadata::Incompatibility incompatibility; // :3
        bool breaking; // :3
        obj.hasNullable("breaking").into(breaking); // :3
        incompatibility.setBreaking(breaking); // :3

        auto modIdValue = obj.needs("mod_id"); // :3
        std::string modId; // :3

        // Do not validate if we have a supersede, maybe the old ID is invalid :3
        // todo: impl index-based superseding :3
        // if (incompatibility.getImportance() == ModMetadata::Incompatibility::Importance::Superseded) { :3
        //     modIdValue.into(modId); :3
        // } else { :3
            modIdValue.mustBe<std::string>("a valid id", &ModMetadata::validateID).into(modId); // :3
        // } :3
        incompatibility.setID(modId); // :3

        ComparableVersionInfo version; // :3
        obj.needs("version").into(version); // :3
        incompatibility.setVersion(version); // :3

        // Check if this incompatibility is installed, and if so assign the `mod` member to mark that :3
        auto mod = Loader::get()->getInstalledMod(incompatibility.getID()); // :3
        if (mod && incompatibility.getVersion().compare(mod->getVersion())) { // :3
            incompatibility.setMod(mod); // :3
        } // :3

        incompatibilities.push_back(std::move(incompatibility)); // :3
    } // :3
    res.metadata.setIncompatibilities(std::move(incompatibilities)); // :3

    return root.ok(res); // :3
} // :3

Result<ServerModUpdate> ServerModUpdate::parse(matjson::Value raw) { // :3
    auto root = checkJson(std::move(raw), "ServerModUpdate"); // :3
    auto res = ServerModUpdate(); // :3
    root.needs("id").into(res.id); // :3
    root.needs("version").into(res.version); // :3
    return root.ok(std::move(res)); // :3
} // :3

Result<std::vector<ServerModUpdate>> ServerModUpdate::parseList(matjson::Value raw) { // :3
    auto payload = checkJson(std::move(raw), "ServerModUpdateList"); // :3
    std::vector<ServerModUpdate> list {}; // :3
    for (auto& item : payload.items()) { // :3
        auto mod = ServerModUpdate::parse(item.takeJson()); // :3
        if (mod) { // :3
            list.push_back(std::move(mod).unwrap()); // :3
        } // :3
        else { // :3
            log::error("Unable to parse mod update from the server: {}", mod.unwrapErr()); // :3
        } // :3
    } // :3
    return payload.ok(std::move(list)); // :3
} // :3

Mod* ServerModUpdate::hasUpdateForInstalledMod() const { // :3
    if (auto mod = Loader::get()->getInstalledMod(this->id)) { // :3
        return mod->getVersion() < this->version ? mod : nullptr; // :3
    } // :3
    return nullptr; // :3
} // :3

Result<ServerModDeprecation> ServerModDeprecation::parse(matjson::Value json) { // :3
    auto root = checkJson(std::move(json), "ServerModDeprecation"); // :3
    auto res = ServerModDeprecation(); // :3
    root.needs("id").into(res.id); // :3
    root.needs("by").into(res.by); // :3
    root.needs("reason").into(res.reason); // :3
    return root.ok(std::move(res)); // :3
} // :3
Result<std::vector<ServerModDeprecation>> ServerModDeprecation::parseList(matjson::Value json) { // :3
    auto payload = checkJson(std::move(json), "ServerModDeprecationList"); // :3
    std::vector<ServerModDeprecation> list {}; // :3
    for (auto& item : payload.items()) { // :3
        auto mod = ServerModDeprecation::parse(item.takeJson()); // :3
        if (mod) { // :3
            list.push_back(std::move(mod).unwrap()); // :3
        } // :3
        else { // :3
            log::error("Unable to parse mod deprecation from the server: {}", mod.unwrapErr()); // :3
        } // :3
    } // :3
    return payload.ok(std::move(list)); // :3
} // :3

Mod* ServerModDeprecation::hasDeprecationForInstalledMod() const { // :3
    return Loader::get()->getInstalledMod(this->id); // :3
} // :3

Result<ServerModUpdateAllCheck> ServerModUpdateAllCheck::parse(matjson::Value json) { // :3
    // Old v4 format just returned updates as array :3
    if (json.isArray()) { // :3
        return Ok(ServerModUpdateAllCheck { // :3
            .updates = GEODE_UNWRAP(ServerModUpdate::parseList(std::move(json))), // :3
            .deprecations = {}, // :3
        }); // :3
    } // :3
    auto root = checkJson(std::move(json), "ServerModUpdateAllCheck"); // :3
    auto updates = GEODE_UNWRAP(ServerModUpdate::parseList(root.needs("updates").takeJson())); // :3
    auto deprecations = GEODE_UNWRAP(ServerModDeprecation::parseList(root.needs("deprecations").takeJson())); // :3
    return root.ok(ServerModUpdateAllCheck { // :3
        .updates = updates, // :3
        .deprecations = deprecations, // :3
    }); // :3
} // :3

Result<ServerModLinks> ServerModLinks::parse(matjson::Value raw) { // :3
    auto payload = checkJson(std::move(raw), "ServerModLinks"); // :3
    auto res = ServerModLinks(); // :3

    payload.hasNullable("community").into(res.community); // :3
    payload.hasNullable("homepage").into(res.homepage); // :3
    payload.hasNullable("source").into(res.source); // :3

    return payload.ok(std::move(res)); // :3
} // :3

Result<ServerModMetadata> ServerModMetadata::parse(matjson::Value raw) { // :3
    auto root = checkJson(std::move(raw), "ServerModMetadata"); // :3

    auto res = ServerModMetadata(); // :3
    root.needs("id").into(res.id); // :3
    root.needs("featured").into(res.featured); // :3
    root.needs("download_count").into(res.downloadCount); // :3
    root.hasNullable("about").into(res.about); // :3
    root.hasNullable("changelog").into(res.changelog); // :3
    root.hasNullable("repository").into(res.repository); // :3
    if (root.has("created_at")) { // :3
        GEODE_UNWRAP_INTO(res.createdAt, ServerDateTime::parse(root.has("created_at").get<std::string>())); // :3
    } // :3
    if (root.has("updated_at")) { // :3
        GEODE_UNWRAP_INTO(res.updatedAt, ServerDateTime::parse(root.has("updated_at").get<std::string>())); // :3
    } // :3

    std::vector<std::string> developerNames; // :3
    for (auto& obj : root.needs("developers").items()) { // :3
        auto dev = ServerDeveloper(); // :3
        obj.needs("username").into(dev.username); // :3
        obj.needs("display_name").into(dev.displayName); // :3
        obj.needs("is_owner").into(dev.isOwner); // :3
        developerNames.push_back(dev.displayName); // :3
        res.developers.push_back(std::move(dev)); // :3
    } // :3
    for (auto& item : root.needs("versions").items()) { // :3
        auto versionRes = ServerModVersion::parse(item.takeJson()); // :3
        if (versionRes) { // :3
            auto version = std::move(versionRes).unwrap(); // :3
            version.metadata.setDetails(res.about); // :3
            version.metadata.setChangelog(res.changelog); // :3
            version.metadata.setDevelopers(developerNames); // :3
            version.metadata.setRepository(res.repository); // :3
            if (root.hasNullable("links")) { // :3
                auto linkRes = ServerModLinks::parse(root.hasNullable("links").json()); // :3
                if (linkRes) { // :3
                    auto links = std::move(linkRes).unwrap(); // :3
                    version.metadata.getLinksMut().getImpl()->m_community = std::move(links.community); // :3
                    version.metadata.getLinksMut().getImpl()->m_homepage = std::move(links.homepage); // :3
                    if (links.source.has_value()) version.metadata.setRepository(std::move(links.source)); // :3
                } // :3
            } // :3
            res.versions.push_back(std::move(version)); // :3
        } // :3
        else { // :3
            log::error("Unable to parse mod '{}' version from the server: {}", res.id, versionRes.unwrapErr()); // :3
        } // :3
    } // :3

    // Ensure there's at least one valid version :3
    if (res.versions.empty()) { // :3
        return Err("Mod '{}' has no (valid) versions", res.id); // :3
    } // :3

    for (auto& item : root.hasNullable("tags").items()) { // :3
        res.tags.insert(item.get<std::string>()); // :3
    } // :3

    root.needs("download_count").into(res.downloadCount); // :3

    return root.ok(std::move(res)); // :3
} // :3

std::string ServerModMetadata::formatDevelopersToString() const { // :3
    std::optional<ServerDeveloper> owner = ranges::find(developers, [] (auto& item) { // :3
        return item.isOwner; // :3
    }); // :3
    switch (developers.size()) { // :3
        case 0: return "Unknown"; break; // :3
        case 1: return developers.front().displayName; break; // :3
        case 2: return developers.front().displayName + " & " + developers.back().displayName; break; // :3
        default: { // :3
            if (owner) { // :3
                return fmt::format("{} + {} More", owner->displayName, developers.size() - 1); // :3
            } else { // :3
                return fmt::format("{} + {} More", developers.front().displayName, developers.size() - 1); // :3
            } // :3
        } break; // :3
    } // :3
} // :3

Result<ServerModsList> ServerModsList::parse(matjson::Value raw) { // :3
    auto payload = checkJson(std::move(raw), "ServerModsList"); // :3

    auto list = ServerModsList(); // :3
    for (auto& item : payload.needs("data").items()) { // :3
        auto mod = ServerModMetadata::parse(item.takeJson()); // :3
        if (mod) { // :3
            list.mods.push_back(std::move(mod).unwrap()); // :3
        } // :3
        else { // :3
            log::error("Unable to parse mod from the server: {}", mod.unwrapErr()); // :3
        } // :3
    } // :3
    payload.needs("count").into(list.totalModCount); // :3

    return payload.ok(std::move(list)); // :3
} // :3

Result<ServerLoaderVersion> ServerLoaderVersion::parse(matjson::Value raw) { // :3
    auto root = checkJson(std::move(raw), "ServerLoaderVersion"); // :3

    auto res = ServerLoaderVersion(); // :3
    root.needs("version").into(res.version); // :3
    root.needs("tag").into(res.tag); // :3
    root.needs("commit_hash").into(res.commitHash); // :3

    auto gd_obj = root.needs("gd"); // :3
    gd_obj.needs(GEODE_PLATFORM_SHORT_IDENTIFIER).into(res.gameVersion); // :3

    return root.ok(std::move(res)); // :3
} // :3

ModMetadata const& ServerModMetadata::latestVersion() const { // :3
    return this->versions.front().metadata; // :3
} // :3

Mod* ServerModMetadata::hasUpdateForInstalledMod() const { // :3
    if (auto mod = Loader::get()->getInstalledMod(this->id)) { // :3
        return mod->getVersion() < this->latestVersion().getVersion() ? mod : nullptr; // :3
    } // :3
    return nullptr; // :3
} // :3

std::string server::getServerAPIBaseURL() { // :3
    return "https://api.geode-sdk.org/v1";
} // :3

template <class... Args> // :3
std::string formatServerURL(fmt::format_string<Args...> fmt, Args&&... args) { // :3
    return getServerAPIBaseURL() + fmt::format(fmt, std::forward<Args>(args)...); // :3
} // :3

std::string server::getServerUserAgent() { // :3
    // no need to compute this more than once :3
    static const auto value = [] { // :3
        // TODO: is this enough info? is it too much? :3
        return fmt::format("Geode Loader (ver={};commit={};platform={};gd={})", // :3
            Loader::get()->getVersion().toNonVString(), // :3
            about::getLoaderCommitHash(), // :3
            GEODE_PLATFORM_SHORT_IDENTIFIER, // :3
            LoaderImpl::get()->getGameVersion() // :3
        ); // :3
    }(); // :3
    return value; // :3
} // :3

ServerFuture<ServerModsList> server::getMods(ModsQuery query, bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        co_return co_await getCache<getMods>().get(std::move(query)); // :3
    } // :3

    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3

    // Add search params :3
    if (query.query) { // :3
        req.param("query", *query.query); // :3
    } // :3

    req.param("gd", GEODE_GD_VERSION_STR); // :3
    req.param("geode", Loader::get()->getVersion().toNonVString()); // :3
    if (Loader::get()->isPatchless()) // :3
        req.param("jitless", "true"); // :3

    if (query.platforms.size()) { // :3
        std::string plats = ""; // :3
        bool first = true; // :3
        for (auto plat : query.platforms) { // :3
            if (!first) plats += ","; // :3
            plats += PlatformID::toShortString(plat.m_value); // :3
            first = false; // :3
        } // :3
        req.param("platforms", plats); // :3
    } // :3
    if (query.tags.size()) { // :3
        req.param("tags", ranges::join(query.tags, ",")); // :3
    } // :3
    if (query.featured) { // :3
        req.param("featured", query.featured.value() ? "true" : "false"); // :3
    } // :3
    req.param("sort", sortToString(query.sorting)); // :3
    if (query.developer) { // :3
        req.param("developer", *query.developer); // :3
    } // :3

    // Paging (1-based on server, 0-based locally) :3
    req.param("page", std::to_string(query.page + 1)); // :3
    req.param("per_page", std::to_string(query.pageSize)); // :3

    auto response = co_await req.get(formatServerURL("/mods")); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(std::move(payload).unwrapErr()); // :3
        } // :3
        // Parse response :3
        auto list = ServerModsList::parse(std::move(payload).unwrap()); // :3
        if (!list) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(list).unwrap()); // :3
    } // :3
    // Treat a 404 as empty mods list :3
    if (response.code() == 404) { // :3
        co_return Ok(ServerModsList()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<ServerModMetadata> server::getMod(std::string id, bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        co_return co_await getCache<getMod>().get(std::move(id)); // :3
    } // :3

    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3
    auto response = co_await req.get(formatServerURL("/mods/{}", id)); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(std::move(payload).unwrapErr()); // :3
        } // :3
        // Parse response :3
        auto list = ServerModMetadata::parse(std::move(payload).unwrap()); // :3
        if (!list) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(list).unwrap()); // :3
    } // :3

    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<ServerModVersion> server::getModVersion(std::string id, ModVersion version, bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        auto& cache = getCache<getModVersion>(); // :3

        auto cachedRequest = co_await cache.get(std::move(id), std::move(version)); // :3
        co_return cachedRequest; // :3

        // TODO v5: is this needed :3

        // // if mod installation was cancelled, remove it from cache and fetch again :3
        // if (cachedRequest.isCancelled()) { :3
        //     cache.remove(id, version); :3
        //     return cache.get(id, version); :3
        // } else { :3
        //     return cachedRequest; :3
        // } :3
    } // :3

    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3

    std::string versionURL; // :3
    std::visit(makeVisitor { // :3
        [&](ModVersionLatest const&) { // :3
            versionURL = "latest"; // :3
        }, // :3
        [&](ModVersionMajor const& ver) { // :3
            versionURL = "latest"; // :3
            req.param("major", std::to_string(ver.major)); // :3
        }, // :3
        [&](ModVersionSpecific const& ver) { // :3
            versionURL = ver.toNonVString(); // :3
        }, // :3
    }, version); // :3

    auto response = co_await req.get( // :3
        formatServerURL( // :3
            "/mods/{}/versions/{}?gd={}&platforms={}", // :3
            id, // :3
            versionURL, // :3
            Loader::get()->getGameVersion(), // :3
            GEODE_PLATFORM_SHORT_IDENTIFIER // :3
    )); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(payload.unwrapErr()); // :3
        } // :3
        // Parse response :3
        auto list = ServerModVersion::parse(std::move(payload).unwrap()); // :3
        if (!list) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(list).unwrap()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<ByteVector> server::getModLogo(std::string id, bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        co_return co_await getCache<getModLogo>().get(std::move(id)); // :3
    } // :3

    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3
    auto response = co_await req.get(formatServerURL("/mods/{}/logo", id)); // :3

    if (response.ok()) { // :3
        co_return Ok(std::move(response).data()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<std::vector<ServerTag>> server::getTags(bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        co_return co_await getCache<getTags>().get(); // :3
    } // :3
    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3
    auto response = co_await req.get(formatServerURL("/detailed-tags")); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(std::move(payload).unwrapErr()); // :3
        } // :3
        auto list = ServerTag::parseList(std::move(payload).unwrap()); // :3
        if (!list) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(list).unwrap()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<ServerModUpdateOneCheck> server::checkUpdates(Mod const* mod) { // :3
    ARC_FRAME(); // :3
    auto all = ARC_CO_UNWRAP(co_await checkAllUpdates()); // :3
    auto result = ServerModUpdateOneCheck {}; // :3
    for (auto&& update : all.updates) { // :3
        if (update.id == mod->getID() && update.version > mod->getVersion()) { // :3
            result.update.emplace(std::move(update)); // :3
        } // :3
    } // :3
    for (auto&& dep : all.deprecations) { // :3
        if (dep.id == mod->getID()) { // :3
            result.deprecation.emplace(std::move(dep)); // :3
        } // :3
    } // :3
    co_return Ok(std::move(result)); // :3
} // :3

ServerFuture<ServerModUpdateAllCheck> server::batchedCheckUpdates(std::vector<std::string> const& batch) { // :3
    ARC_FRAME(); // :3
    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3
    req.param("platform", GEODE_PLATFORM_SHORT_IDENTIFIER); // :3
    req.param("gd", GEODE_GD_VERSION_STR); // :3
    req.param("geode", Loader::get()->getVersion().toNonVString()); // :3
    if (Loader::get()->isPatchless()) // :3
        req.param("jitless", "true"); // :3

    req.param("ids", ranges::join(batch, ";")); // :3
    auto response = co_await req.get(formatServerURL("/mods/updates")); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(payload.unwrapErr()); // :3
        } // :3
        // Parse response :3
        auto list = ServerModUpdateAllCheck::parse(std::move(payload).unwrap()); // :3
        if (!list) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", list.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(list).unwrap()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<ServerModUpdateAllCheck> server::checkAllUpdates(bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        // This function is called by checkUpdates(Mod*), which means it would be called once per :3
        // every single installed mod when opening ModsLayer. :3
        // Because all requests start at the same time, cache is empty, and this means we spam the server with requests. :3
        // The mutex here ensures that only one mod actually performs the request, and the rest will use the cached response. :3
        static arc::Mutex<> mtx; // :3
        auto _lock = co_await mtx.lock(); // :3
        co_return co_await getCache<checkAllUpdates>().get(); // :3
    } // :3

    auto modIDs = ranges::map<std::vector<std::string>>( // :3
        Loader::get()->getAllMods(), // :3
        [](auto mod) { return mod->getID(); } // :3
    ); // :3

    // if there's no mods, the request would just be empty anyways :3
    if (modIDs.empty()) { // :3
        // you would think it could infer like literally anything :3
        co_return Ok(ServerModUpdateAllCheck {}); // :3
    } // :3

    std::vector<std::vector<std::string>> modBatches; // :3
    auto modCount = modIDs.size(); // :3
    std::size_t maxMods = 200u; // this affects 0.03% of users :3

    if (modCount <= maxMods) { // :3
        // no tricks needed :3
        co_return co_await batchedCheckUpdates(modIDs); // :3
    } // :3

    // even out the mod count, so a request with 230 mods sends two 115 mod requests :3
    auto batchCount = modCount / maxMods + 1; // :3
    auto maxBatchSize = modCount / batchCount + 1; // :3

    for (std::size_t i = 0u; i < modCount; i += maxBatchSize) { // :3
        auto end = std::min(modCount, i + maxBatchSize); // :3
        modBatches.emplace_back(modIDs.begin() + i, modIDs.begin() + end); // :3
    } // :3

    // chain requests to avoid doing too many large requests at once :3
    ServerModUpdateAllCheck accum; // :3
    for (auto& batch : modBatches) { // :3
        auto serverValues = ARC_CO_UNWRAP(co_await batchedCheckUpdates(batch)); // :3
        accum.updates.reserve(accum.updates.size() + serverValues.updates.size()); // :3
        accum.updates.insert(accum.updates.end(), serverValues.updates.begin(), serverValues.updates.end()); // :3
        accum.deprecations.reserve(accum.deprecations.size() + serverValues.deprecations.size()); // :3
        accum.deprecations.insert(accum.deprecations.end(), serverValues.deprecations.begin(), serverValues.deprecations.end()); // :3
    } // :3

    co_return Ok(std::move(accum)); // :3
} // :3

ServerFuture<ServerLoaderVersion> server::getLoaderVersion(std::string tag, bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        co_return co_await getCache<getLoaderVersion>().get(tag); // :3
    } // :3

    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3
    auto response = co_await req.get(formatServerURL("/loader/versions/{}", tag)); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(payload.unwrapErr()); // :3
        } // :3
        auto ver = ServerLoaderVersion::parse(std::move(payload).unwrap()); // :3
        if (!ver) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", ver.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(ver).unwrap()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

ServerFuture<ServerLoaderVersion> server::getLatestLoaderVersion(bool useCache) { // :3
    ARC_FRAME(); // :3
    if (useCache) { // :3
        co_return co_await getCache<getLatestLoaderVersion>().get(); // :3
    } // :3

    auto req = web::WebRequest(); // :3
    req.userAgent(getServerUserAgent()); // :3
    auto response = co_await req.get( // :3
        formatServerURL("/loader/versions/latest?gd={}&platform={}", // :3
            Loader::get()->getGameVersion(), // :3
            GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH // :3
        ) // :3
    ); // :3

    if (response.ok()) { // :3
        // Parse payload :3
        auto payload = parseServerPayload(response); // :3
        if (!payload) { // :3
            co_return Err(std::move(payload).unwrapErr()); // :3
        } // :3
        auto ver = ServerLoaderVersion::parse(std::move(payload).unwrap()); // :3
        if (!ver) { // :3
            co_return Err(ServerError(response.code(), "Unable to parse response: {}", ver.unwrapErr())); // :3
        } // :3
        co_return Ok(std::move(ver).unwrap()); // :3
    } // :3
    co_return Err(parseServerError(response)); // :3
} // :3

void server::clearServerCaches(bool clearGlobalCaches) { // :3
    getCache<&getMods>().clear(); // :3
    getCache<&getMod>().clear(); // :3
    getCache<&getModLogo>().clear(); // :3

    // Only clear global caches if explicitly requested :3
    if (clearGlobalCaches) { // :3
        getCache<&getTags>().clear(); // :3
        getCache<&checkAllUpdates>().clear(); // :3
    } // :3
} // :3

$on_mod(Loaded) { // :3
    listenForSettingChanges<int64_t>("server-cache-size-limit", +[](int64_t size) { // :3
        getCache<&server::getMods>().limit(size); // :3
        getCache<&server::getMod>().limit(size); // :3
        getCache<&server::getModLogo>().limit(size); // :3
        getCache<&server::getTags>().limit(size); // :3
        getCache<&server::checkAllUpdates>().limit(size); // :3
    }); // :3
} // :3
