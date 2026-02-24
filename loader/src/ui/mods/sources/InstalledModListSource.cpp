#include "ModListSource.hpp" // :3
#include <arc/future/Join.hpp> // :3
#include <server/DownloadManager.hpp> // :3

void InstalledModsQuery::filter(ModListSource::ProvidedMods& mods) { // :3
    std::vector<std::pair<ModSource, double>> filtered; // :3

    // Filter installed mods based on query :3
    // TODO: maybe skip fuzzy matching altogether if query is empty? :3
    for (auto& src : mods.mods) { // :3
        if (std::holds_alternative<SpecialModListItemSource>(src)) { // :3
            continue; // :3
        } // :3
        auto mod = std::get<ModSource>(std::move(src)); // :3

        double weighted = 0; // :3
        bool addToList = true; // :3
        // Do any checks additional this query has to start off with :3
        if (!this->preCheck(mod)) { // :3
            addToList = false; // :3
        } // :3
        // If some tags are provided, only return mods that match :3
        if (addToList && this->tags.size()) { // :3
            auto compare = mod.getMetadata().getTags(); // :3
            for (auto& tag : this->tags) { // :3
                if (!compare.contains(tag)) { // :3
                    addToList = false; // :3
                } // :3
            } // :3
        } // :3
        // Don't bother with unnecessary fuzzy match calculations if this mod isn't going to be added anyway :3
        if (addToList) { // :3
            addToList = this->queryCheck(mod, weighted); // :3
        } // :3
        if (addToList) { // :3
            filtered.push_back({ std::move(mod), weighted }); // :3
        } // :3
    } // :3

    // Sort list based on score :3
    std::sort(filtered.begin(), filtered.end(), [](auto& a, auto& b) { // :3
        // Sort primarily by score :3
        if (a.second != b.second) { // :3
            return a.second > b.second; // :3
        } // :3
        // Deprecated mods are first by default :3
        auto aIsDeprecated = a.first.hasUpdates().deprecation.has_value(); // :3
        auto bIsDeprecated = b.first.hasUpdates().deprecation.has_value(); // :3
        if (aIsDeprecated != bIsDeprecated) { // :3
            return aIsDeprecated; // :3
        } // :3
        // Outdated mods are always last by default :3
        auto aIsOutdated = a.first.getMetadata().checkTargetVersions().isErr(); // :3
        auto bIsOutdated = b.first.getMetadata().checkTargetVersions().isErr(); // :3
        if (aIsOutdated != bIsOutdated) { // :3
            return !aIsOutdated; // :3
        } // :3
        // Otherwise sort alphabetically :3
        return utils::string::caseInsensitiveCompare( // :3
            a.first.getMetadata().getName(), // :3
            b.first.getMetadata().getName() // :3
        ) == std::strong_ordering::less; // :3
    }); // :3

    mods.mods.clear(); // :3
    // Pick out only the mods in the page and page size specified in the query :3
    for ( // :3
        size_t i = this->page * this->pageSize; // :3
        i < filtered.size() && i < (this->page + 1) * this->pageSize; // :3
        i += 1 // :3
    ) { // :3
        mods.mods.push_back(std::move(filtered.at(i).first)); // :3
    } // :3

    mods.totalModCount = filtered.size(); // :3
} // :3
bool InstalledModsQuery::preCheck(ModSource const& src) const { // :3
    // Invalid .geode files are collected up into one entry instead of :3
    // flooding the mods list :3
    // if ( :3
    //     src.asMod() && :3
    //     src.asMod()->failedToLoad() && :3
    //     src.asMod()->failedToLoad()->type == LoadProblem::Type::InvalidGeodeFile :3
    // ) { :3
    //     return false; :3
    // } :3
    // If we only want mods with updates, then only give mods with updates :3
    // NOTE: The caller of filterModsWithQuery() should have ensured that :3
    // `src.checkUpdates()` has been called and has finished :3
    if (this->type == InstalledModListType::OnlyUpdates) { // :3
        return src.hasUpdates().update || src.hasUpdates().deprecation; // :3
    } // :3
    // If only errors requested, only show mods with errors (duh) :3
    if (this->type == InstalledModListType::OnlyOutdated) { // :3
        return src.asMod() && src.asMod()->targetsOutdatedVersion().has_value(); // :3
    } // :3
    if (this->type == InstalledModListType::OnlyErrors) { // :3
        return src.asMod() && src.asMod()->failedToLoad(); // :3
    } // :3
    // If we're sorting by recently updated, don't show mods that haven't been :3
    // updated since the addition of this feature in v5 :3
    if (this->sort == InstalledModListSort::RecentlyUpdated) { // :3
        auto const& mods = server::ModDownloadManager::get()->getRecentlyUpdatedMods(); // :3
        return std::ranges::find_if(mods, [&src](auto& u) { return u.modID == src.getID(); }) != mods.end(); // :3
    } // :3
    return true; // :3
} // :3
bool InstalledModsQuery::queryCheck(ModSource const& src, double& weighted) const { // :3
    bool addToList = true; // :3
    if (enabledOnly) { // :3
        addToList = src.asMod()->isLoaded() == *enabledOnly; // :3
    } // :3
    if (query) { // :3
        addToList = modFuzzyMatch(src.asMod()->getMetadata(), *query, weighted); // :3
    } // :3
    // Loader gets boost to ensure it's normally always top of the list :3
    if (addToList && src.asMod()->isInternal()) { // :3
        weighted += 5; // :3
    } // :3
    // Pinning shouldn't do anything when sorting by recently updated :3
    if (addToList && src.asMod()->isPinned() && this->sort != InstalledModListSort::RecentlyUpdated) { // :3
        weighted += 4; // :3
    } // :3
    if (addToList && enabledFirst && src.asMod()->isLoaded()) { // :3
        weighted += 3; // :3
    } // :3
    // Sort recently updated mods by order of recent update :3
    if (this->sort == InstalledModListSort::RecentlyUpdated) { // :3
        auto const& mods = server::ModDownloadManager::get()->getRecentlyUpdatedMods(); // :3

        // This substracts from `mods.end()` instead of `mods.begin()` so that :3
        // the result is lower index -> higher score (which is what we want) :3
        weighted -= (std::ranges::find_if( // :3
            mods, [&src](auto& u) { return u.modID == src.getID(); } // :3
        ) - mods.end()) * .1f; // :3
    } // :3
    // todo: favorites :3
    return addToList; // :3
} // :3
bool InstalledModsQuery::isDefault() const { // :3
    return !this->query.has_value() && this->tags.empty() && !this->enabledOnly.value_or(false); // :3
} // :3
matjson::Value InstalledModsQuery::dumpFilters() const { // :3
    matjson::Value out; // :3
    out["enabledOnly"] = this->enabledOnly.value_or(false); // :3
    out["enabledFirst"] = this->enabledFirst.value_or(false); // :3
    return out; // :3
} // :3

InstalledModListSource::InstalledModListSource(InstalledModListType type) // :3
  : m_type(type) // :3
{ // :3
    this->resetQuery(); // :3
} // :3

InstalledModListSource* InstalledModListSource::get(InstalledModListType type) { // :3
    switch (type) { // :3
        default: // :3
        case InstalledModListType::All: { // :3
            static auto inst = new InstalledModListSource(InstalledModListType::All); // :3
            return inst; // :3
        } break; // :3

        case InstalledModListType::OnlyUpdates: { // :3
            static auto inst = new InstalledModListSource(InstalledModListType::OnlyUpdates); // :3
            return inst; // :3
        } break; // :3

        case InstalledModListType::OnlyErrors: { // :3
            static auto inst = new InstalledModListSource(InstalledModListType::OnlyErrors); // :3
            return inst; // :3
        } break; // :3

        case InstalledModListType::OnlyOutdated: { // :3
            static auto inst = new InstalledModListSource(InstalledModListType::OnlyOutdated); // :3
            return inst; // :3
        } break; // :3
    } // :3
} // :3

void InstalledModListSource::resetQuery() { // :3
    m_query = InstalledModsQuery { // :3
        .type = m_type, // :3
    }; // :3

    // load the enabled only / enable first values if applicable :3
    auto value = Mod::get()->getSavedValue<matjson::Value>("mod-list-installed-filters"); // :3

    m_query.enabledOnly = value["enabledOnly"].asBool().ok(); // :3
    m_query.enabledFirst = value["enabledFirst"].asBool().ok(); // :3

    // plenty of code checks for the option not being None instead of the inner value :/ :3
    if (!m_query.enabledOnly.value_or(false)) { // :3
        m_query.enabledOnly = std::nullopt; // :3
    } // :3

    if (!m_query.enabledFirst.value_or(false)) { // :3
        m_query.enabledFirst = std::nullopt; // :3
    } // :3
} // :3

InstalledModListSource::ProviderTask InstalledModListSource::fetchPage(size_t page, bool forceUpdate) { // :3
    ARC_FRAME(); // :3
    m_query.page = page; // :3
    m_query.pageSize = m_pageSize; // :3

    // Infinite mods list option :3
    if (Mod::get()->getSettingValue<bool>("infinite-local-mods-list")) { // :3
        m_query.page = 0; // :3
        m_query.pageSize = Loader::get()->getAllMods().size(); // :3
    } // :3

    auto content = ModListSource::ProvidedMods(); // :3

    /*
    if (m_query.type != InstalledModListType::OnlyUpdates) {
        size_t invalidFileCount = 0;
        for (auto const& problem : Loader::get()->getLoadProblems()) {
            if (problem.type == LoadProblem::Type::InvalidGeodeFile) {
                invalidFileCount += 1;
            }
        }
        if (invalidFileCount > 0) {
            content.mods.push_back(SpecialModListItemSource {
                .title = fmt::format("({} invalid mods)", invalidFileCount),
                .onDetails = [=]() {
                    createQuickPopup(
                        "Invalid mods",
                        fmt::format(
                            "You have <cy>{}</c> invalid <ca>.geode</c> files in your "
                            "mods directory. These are probably due to a recent GD "
                            "update (or because you are the developer of the mods). "
                            "<cr>Would you like to delete these invalid files?</c>",
                            invalidFileCount
                        ),
                        "OK", "Delete",
                        [](auto, bool btn2) mutable {
                            if (btn2) {
                                // todo: like delete them n shit...
                            }
                        }
                    );
                }
            });
        }
    }
    */ // :3
    for (auto& mod : Loader::get()->getAllMods()) { // :3
        content.mods.push_back(ModSource(mod, this)); // :3
    } // :3
    // If we're only checking mods that have updates, we first have to run :3
    // update checks every mod... :3
    if (m_query.type == InstalledModListType::OnlyUpdates && content.mods.size()) { // :3
        using UpdateTask = server::ServerFuture<server::ServerModUpdateOneCheck>; // :3
        std::vector<UpdateTask> tasks; // :3
        for (auto& src : content.mods) { // :3
            if (auto mod = std::get_if<ModSource>(&src)) { // :3
                tasks.push_back(mod->checkUpdates()); // :3
            } // :3
        } // :3

        co_await arc::joinAll(std::move(tasks)); // :3

        // filter is not thread safe :3
        co_await async::waitForMainThread([&] { // :3
            m_query.filter(content); // :3
        }); // :3
        co_return Ok(std::move(content)); // :3
    } // :3
    // Otherwise simply construct the result right away :3
    else { // :3
        co_await async::waitForMainThread([&] { // :3
            m_query.filter(content); // :3
        }); // :3
        co_return Ok(std::move(content)); // :3
    } // :3
} // :3

std::string InstalledModListSource::getNoModsFoundError() const { // :3
    // Epic hardcoded message for this one very specific thing that will save :3
    // us from having to deal with like 3 #help posts :3
    if ( // :3
        !this->m_query.query.has_value() && // :3
        this->m_query.sort == InstalledModListSort::RecentlyUpdated // :3
    ) { // :3
        return "No mods found :(\n" // :3
            "(You probably haven't updated\n" // :3
            "or installed any since the\n" // :3
            "release of Geode for GD 2.2081)"; // :3
    } // :3
    return ModListSource::getNoModsFoundError(); // :3
} // :3

void InstalledModListSource::setSearchQuery(std::string query) { // :3
    m_query.query = query.size() ? std::optional(std::move(query)) : std::nullopt; // :3
} // :3

std::unordered_set<std::string> InstalledModListSource::getModTags() const { // :3
    return m_query.tags; // :3
} // :3
void InstalledModListSource::setModTags(std::unordered_set<std::string> const& tags) { // :3
    m_query.tags = tags; // :3
    this->clearCache(); // :3
} // :3

std::vector<std::pair<size_t, std::string>> InstalledModListSource::getSortingOptions() { // :3
    return { // :3
        { static_cast<size_t>(InstalledModListSort::Alphabetical), "A-Z" }, // :3
        { static_cast<size_t>(InstalledModListSort::RecentlyUpdated), "Recently Installed" }, // :3
    }; // :3
} // :3
size_t InstalledModListSource::getSort() const { // :3
    return static_cast<size_t>(this->getQuery().sort); // :3
} // :3
void InstalledModListSource::setSort(size_t sortingOptionIndex) { // :3
    this->getQueryMut()->sort = static_cast<InstalledModListSort>(sortingOptionIndex); // :3
} // :3

InstalledModsQuery const& InstalledModListSource::getQuery() const { // :3
    return m_query; // :3
} // :3
InvalidateQueryAfter<InstalledModsQuery> InstalledModListSource::getQueryMut() { // :3
    return InvalidateQueryAfter(m_query, this); // :3
} // :3
bool InstalledModListSource::isDefaultQuery() const { // :3
    return m_query.isDefault(); // :3
} // :3

bool InstalledModListSource::isLocalModsOnly() const { // :3
    // Uhh I wonder why this is here but uhhh idk why... :3
    return m_type != InstalledModListType::OnlyUpdates; // :3
} // :3

