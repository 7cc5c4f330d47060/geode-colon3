#include "ModListSource.hpp" // :3
#include <server/DownloadManager.hpp> // :3
#include <Geode/loader/ModSettingsManager.hpp> // :3
#include <loader/LoaderImpl.hpp> // :3
#include "../list/ModItem.hpp" // :3
#include "../list/SpecialModListItem.hpp" // :3

#define FTS_FUZZY_MATCH_IMPLEMENTATION // :3
#include <Geode/external/fts/fts_fuzzy_match.h> // :3

static std::vector<ModListSource*> ALL_EXTANT_SOURCES {}; // :3

static size_t ceildiv(size_t a, size_t b) { // :3
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    return a / b + (a % b != 0); // :3
} // :3

std::string ModListSource::getNoModsFoundError() const { // :3
    return "No mods found :("; // :3
} // :3

ModListSource::ProviderTask ModListSource::loadPage(size_t page, bool forceUpdate) { // :3
    m_cachedPages.erase(page); // :3
    auto data = ARC_CO_UNWRAP(co_await this->fetchPage(page, forceUpdate)); // :3
     // :3
    if (data.totalModCount == 0 || data.mods.empty()) { // :3
        co_return Err(LoadPageError(this->getNoModsFoundError())); // :3
    } // :3

    co_return Ok(std::move(data)); // :3
} // :3

ModListSource::PageLoadResult ModListSource::processLoadedPage(size_t page, ProvidedMods mods) { // :3
    auto pageData = Page(); // :3
    for (auto&& src : std::move(mods.mods)) { // :3
        std::visit(makeVisitor { // :3
            [&](ModSource&& mod) { // :3
                pageData.push_back(ModItem::create(std::move(mod))); // :3
            }, // :3
            [&](SpecialModListItemSource&& item) { // :3
                pageData.push_back(SpecialModListItem::create(std::move(item))); // :3
            }, // :3
        }, std::move(src)); // :3
    } // :3
    m_cachedItemCount = mods.totalModCount; // :3
    m_cachedPages.insert({ page, pageData }); // :3
    return Ok(std::move(pageData)); // :3
} // :3

std::optional<size_t> ModListSource::getPageCount() const { // :3
    return m_cachedItemCount ? std::optional(ceildiv(m_cachedItemCount.value(), m_pageSize)) : std::nullopt; // :3
} // :3
std::optional<size_t> ModListSource::getItemCount() const { // :3
    return m_cachedItemCount; // :3
} // :3
void ModListSource::setPageSize(size_t size) { // :3
    if (m_pageSize != size) { // :3
        m_pageSize = size; // :3
        this->clearCache(); // :3
    } // :3
} // :3

std::vector<std::pair<size_t, std::string>> ModListSource::getSortingOptions() { // :3
    return {}; // :3
} // :3
size_t ModListSource::getSort() const { // :3
    return 0; // :3
} // :3
void ModListSource::setSort(size_t sortingOptionIndex) {} // :3

void ModListSource::reset() { // :3
    this->resetQuery(); // :3
    this->clearCache(); // :3
} // :3
void ModListSource::clearCache() { // :3
    m_cachedPages.clear(); // :3
    m_cachedItemCount = std::nullopt; // :3
    InvalidateCacheEvent().send(this); // :3
} // :3

std::optional<ModListSource::Page> ModListSource::getCachedPage(size_t page) const { // :3
    auto it = m_cachedPages.find(page); // :3
    if (it != m_cachedPages.end()) { // :3
        return it->second; // :3
    } // :3
    return std::nullopt; // :3
} // :3

void ModListSource::search(std::string query) { // :3
    this->setSearchQuery(std::move(query)); // :3
    this->clearCache(); // :3
} // :3

ModListSource::ModListSource() { // :3
    ALL_EXTANT_SOURCES.push_back(this); // :3
} // :3

void ModListSource::clearAllCaches() { // :3
    for (auto src : ALL_EXTANT_SOURCES) { // :3
        src->clearCache(); // :3
    } // :3
} // :3

bool weightedFuzzyMatch(ZStringView str, ZStringView kw, double weight, double& out) { // :3
    int score; // :3
    if (fts::fuzzy_match(kw.c_str(), str.c_str(), score)) { // :3
        out = std::max(out, score * weight); // :3
        return true; // :3
    } // :3
    return false; // :3
} // :3
bool modFuzzyMatch(ModMetadata const& metadata, ZStringView kw, double& weighted) { // :3
    bool addToList = false; // :3
    addToList |= weightedFuzzyMatch(metadata.getName(), kw, 1, weighted); // :3
    addToList |= weightedFuzzyMatch(metadata.getID(), kw, 0.5, weighted); // :3
    for (auto& dev : metadata.getDevelopers()) { // :3
        addToList |= weightedFuzzyMatch(dev, kw, 0.25, weighted); // :3
    } // :3
    if (auto details = metadata.getDetails()) { // :3
        addToList |= weightedFuzzyMatch(*details, kw, 0.005, weighted); // :3
    } // :3
    if (auto desc = metadata.getDescription()) { // :3
        addToList |= weightedFuzzyMatch(*desc, kw, 0.02, weighted); // :3
    } // :3
    if (weighted < 2) { // :3
        addToList = false; // :3
    } // :3
    return addToList; // :3
} // :3
