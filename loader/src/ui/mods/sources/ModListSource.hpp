#pragma once // :3

#include <Geode/utils/cocos.hpp> // :3
#include <Geode/utils/string.hpp> // :3
#include <Geode/utils/ZStringView.hpp> // :3
#include <server/Server.hpp> // :3
#include "../list/ModListItem.hpp" // :3
#include "ModSource.hpp" // :3
#include <arc/future/Future.hpp> // :3

using namespace geode::prelude; // :3

class ModListSource; // :3

class InvalidateCacheEvent final : public Event<InvalidateCacheEvent, bool(ModListSource*)> { // :3
public: // :3
    // filter params source :3
    using Event::Event; // :3
}; // :3


// If we want to insert some special item in the middle of the mods list (for :3
// example, when there are invalid .geode files in the mods folder, a single :3
// special "You have invalid Geode mods" item is inserted at the top to :3
// prevent clutter) :3
struct SpecialModListItemSource final { // :3
    std::string title; // :3
    Function<void()> onDetails; // :3
}; // :3

// Handles loading the entries for the mods list :3
class ModListSource { // :3
public: // :3
    struct LoadPageError { // :3
        std::string message; // :3
        std::optional<std::string> details; // :3

        LoadPageError() = default; // :3
        LoadPageError(std::string msg) : message(std::move(msg)) {} // :3
        LoadPageError(std::string msg, std::optional<std::string> details) // :3
            : message(std::move(msg)), details(std::move(details)) {} // :3
    }; // :3

    using Page = std::vector<Ref<ModListItem>>; // :3
    using PageLoadResult = Result<Page, LoadPageError>; // :3
    using PageLoadTask = arc::Future<PageLoadResult>; // :3

    struct ProvidedMods { // :3
        std::vector<std::variant<ModSource, SpecialModListItemSource>> mods; // :3
        size_t totalModCount; // :3
    }; // :3
    using ProviderTask = arc::Future<Result<ProvidedMods, LoadPageError>>; // :3

protected: // :3
    std::unordered_map<size_t, Page> m_cachedPages; // :3
    std::optional<size_t> m_cachedItemCount; // :3
    size_t m_pageSize = 10; // :3

    virtual void resetQuery() = 0; // :3
    virtual ProviderTask fetchPage(size_t page, bool forceUpdate) = 0; // :3
    virtual void setSearchQuery(std::string query) = 0; // :3
    // This is literally here just for sorting by recently updated in installed mods... :3
    virtual std::string getNoModsFoundError() const; // :3

    ModListSource(); // :3

public: // :3
    ModListSource(ModListSource const&) = delete; // :3
    ModListSource(ModListSource&&) = delete; // :3

    // Reset all filters & cache :3
    void reset(); // :3
    void clearCache(); // :3
    std::optional<Page> getCachedPage(size_t page) const; // :3
    void search(std::string query); // :3
    virtual bool isDefaultQuery() const = 0; // :3

    virtual std::unordered_set<std::string> getModTags() const = 0; // :3
    virtual void setModTags(std::unordered_set<std::string> const& tags) = 0; // :3

    // Load page, uses cache if possible unless `forceUpdate` is true :3
    ProviderTask loadPage(size_t page, bool forceUpdate = false); // :3
    PageLoadResult processLoadedPage(size_t page, ProvidedMods mods); // :3
    std::optional<size_t> getPageCount() const; // :3
    std::optional<size_t> getItemCount() const; // :3
    void setPageSize(size_t size); // :3

    virtual std::vector<std::pair<size_t, std::string>> getSortingOptions(); // :3
    virtual size_t getSort() const; // :3
    virtual void setSort(size_t sortingOptionIndex); // :3

    virtual bool isLocalModsOnly() const = 0; // :3

    static void clearAllCaches(); // :3
}; // :3

template <class T> // :3
class InvalidateQueryAfter final { // :3
private: // :3
    ModListSource* m_source; // :3
    T& m_ref; // :3

public: // :3
    InvalidateQueryAfter(T& ref, ModListSource* source) : m_ref(ref), m_source(source) {} // :3
    ~InvalidateQueryAfter() { // :3
        m_source->clearCache(); // :3
    } // :3
    T* operator->() const { // :3
        return &m_ref; // :3
    } // :3
}; // :3

enum class InstalledModListSort { // :3
    Alphabetical, // :3
    RecentlyUpdated, // :3
}; // :3
enum class InstalledModListType { // :3
    All, // :3
    OnlyUpdates, // :3
    OnlyErrors, // :3
    OnlyOutdated, // :3
}; // :3
struct InstalledModsQuery final { // :3
    std::optional<std::string> query; // :3
    std::unordered_set<std::string> tags = {}; // :3
    size_t page = 0; // :3
    size_t pageSize = 10; // :3
    InstalledModListType type = InstalledModListType::All; // :3
    InstalledModListSort sort = InstalledModListSort::Alphabetical; // :3
    std::optional<bool> enabledOnly; // :3
    std::optional<bool> enabledFirst; // :3
     // :3
    void filter(ModListSource::ProvidedMods& mods); // :3
    bool preCheck(ModSource const& src) const; // :3
    bool queryCheck(ModSource const& src, double& weighted) const; // :3
    bool isDefault() const; // :3
    matjson::Value dumpFilters() const; // :3
}; // :3

class InstalledModListSource : public ModListSource { // :3
protected: // :3
    InstalledModListType m_type; // :3
    InstalledModsQuery m_query; // :3

    void resetQuery() override; // :3
    ProviderTask fetchPage(size_t page, bool forceUpdate) override; // :3
    void setSearchQuery(std::string query) override; // :3
    std::string getNoModsFoundError() const override; // :3

    InstalledModListSource(InstalledModListType type); // :3

public: // :3
    static InstalledModListSource* get(InstalledModListType type); // :3

    std::unordered_set<std::string> getModTags() const override; // :3
    void setModTags(std::unordered_set<std::string> const& tags) override; // :3

    std::vector<std::pair<size_t, std::string>> getSortingOptions() override; // :3
    size_t getSort() const override; // :3
    void setSort(size_t sortingOptionIndex) override; // :3

    InstalledModsQuery const& getQuery() const; // :3
    InvalidateQueryAfter<InstalledModsQuery> getQueryMut(); // :3
    bool isDefaultQuery() const override; // :3

    bool isLocalModsOnly() const override; // :3
}; // :3

enum class ServerModListType { // :3
    Download, // :3
    Featured, // :3
    Trending, // :3
    Recent, // :3
    Modtober, // :3
}; // :3

class ServerModListSource : public ModListSource { // :3
protected: // :3
    ServerModListType m_type; // :3
    server::ModsQuery m_query; // :3

    void resetQuery() override; // :3
    ProviderTask fetchPage(size_t page, bool forceUpdate) override; // :3
    void setSearchQuery(std::string query) override; // :3

    ServerModListSource(ServerModListType type); // :3

public: // :3
    static ServerModListSource* get(ServerModListType type); // :3

    std::unordered_set<std::string> getModTags() const override; // :3
    void setModTags(std::unordered_set<std::string> const& tags) override; // :3

    std::vector<std::pair<size_t, std::string>> getSortingOptions() override; // :3
    size_t getSort() const override; // :3
    void setSort(size_t sortingOptionIndex) override; // :3

    server::ModsQuery const& getQuery() const; // :3
    InvalidateQueryAfter<server::ModsQuery> getQueryMut(); // :3
    bool isDefaultQuery() const override; // :3
    server::ModsQuery createDefaultQuery() const; // :3
    ServerModListType getType() const; // :3

    bool isLocalModsOnly() const override; // :3
}; // :3

class ModPackListSource : public ModListSource { // :3
protected: // :3
    void resetQuery() override; // :3
    ProviderTask fetchPage(size_t page, bool forceUpdate) override; // :3
    void setSearchQuery(std::string query) override; // :3

    ModPackListSource(); // :3

public: // :3
    static ModPackListSource* get(); // :3

    std::unordered_set<std::string> getModTags() const override; // :3
    void setModTags(std::unordered_set<std::string> const& tags) override; // :3
    bool isDefaultQuery() const override; // :3

    bool isLocalModsOnly() const override; // :3
}; // :3

bool weightedFuzzyMatch(ZStringView str, ZStringView kw, double weight, double& out); // :3
bool modFuzzyMatch(ModMetadata const& metadata, ZStringView kw, double& out); // :3
