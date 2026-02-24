#include "ModListSource.hpp" // :3

void ServerModListSource::resetQuery() { // :3
    m_query = this->createDefaultQuery(); // :3
} // :3

ServerModListSource::ProviderTask ServerModListSource::fetchPage(size_t page, bool forceUpdate) { // :3
    m_query.page = page; // :3
    m_query.pageSize = m_pageSize; // :3
    auto result = co_await server::getMods(m_query, !forceUpdate); // :3
    if (!result) { // :3
        co_return Err(LoadPageError("Error loading mods", result.unwrapErr().details)); // :3
    }  // :3

    auto list = std::move(result).unwrap(); // :3
    auto content = ModListSource::ProvidedMods(); // :3
    for (auto&& mod : std::move(list.mods)) { // :3
        content.mods.push_back(ModSource(std::move(mod), this)); // :3
    } // :3
    content.totalModCount = list.totalModCount; // :3
    co_return Ok(std::move(content)); // :3
} // :3

ServerModListSource::ServerModListSource(ServerModListType type) // :3
  : m_type(type) // :3
{ // :3
    this->resetQuery(); // :3
} // :3

ServerModListSource* ServerModListSource::get(ServerModListType type) { // :3
    switch (type) { // :3
        default: [[fallthrough]]; // :3
        case ServerModListType::Download: { // :3
            static auto inst = new ServerModListSource(ServerModListType::Download); // :3
            return inst; // :3
        } break; // :3

        case ServerModListType::Featured: { // :3
            static auto inst = new ServerModListSource(ServerModListType::Featured); // :3
            return inst; // :3
        } break; // :3

        case ServerModListType::Trending: { // :3
            static auto inst = new ServerModListSource(ServerModListType::Trending); // :3
            return inst; // :3
        } break; // :3

        case ServerModListType::Recent: { // :3
            static auto inst = new ServerModListSource(ServerModListType::Recent); // :3
            return inst; // :3
        } break; // :3

        case ServerModListType::Modtober: { // :3
            static auto inst = new ServerModListSource(ServerModListType::Modtober); // :3
            return inst; // :3
        } break; // :3
    } // :3
} // :3

void ServerModListSource::setSearchQuery(std::string query) { // :3
    if (query.empty()) { // :3
        m_query.query = std::nullopt; // :3
        m_query.platforms = { GEODE_PLATFORM_TARGET }; // :3
    } else { // :3
        m_query.query = std::optional(std::move(query)); // :3
        m_query.platforms = {}; // :3
    } // :3
} // :3

std::unordered_set<std::string> ServerModListSource::getModTags() const { // :3
    return m_query.tags; // :3
} // :3
void ServerModListSource::setModTags(std::unordered_set<std::string> const& tags) { // :3
    m_query.tags = tags; // :3
    this->clearCache(); // :3
} // :3

std::vector<std::pair<size_t, std::string>> ServerModListSource::getSortingOptions() { // :3
    return { // :3
        { static_cast<size_t>(server::ModsSort::Downloads), "Most Downloads" }, // :3
        { static_cast<size_t>(server::ModsSort::RecentlyPublished), "Recently Published" }, // :3
        { static_cast<size_t>(server::ModsSort::RecentlyUpdated), "Recently Updated" }, // :3
    }; // :3
} // :3
size_t ServerModListSource::getSort() const { // :3
    return static_cast<size_t>(this->getQuery().sorting); // :3
} // :3
void ServerModListSource::setSort(size_t sortingOptionIndex) { // :3
    this->getQueryMut()->sorting = static_cast<server::ModsSort>(sortingOptionIndex);  // :3
} // :3

server::ModsQuery const& ServerModListSource::getQuery() const { // :3
    return m_query; // :3
} // :3
InvalidateQueryAfter<server::ModsQuery> ServerModListSource::getQueryMut() { // :3
    return InvalidateQueryAfter(m_query, this); // :3
} // :3
bool ServerModListSource::isDefaultQuery() const { // :3
    return m_query == this->createDefaultQuery(); // :3
} // :3

server::ModsQuery ServerModListSource::createDefaultQuery() const { // :3
    switch (m_type) { // :3
        case ServerModListType::Download: return server::ModsQuery {}; // :3

        case ServerModListType::Featured: return server::ModsQuery { // :3
            .featured = true, // :3
        }; // :3

        case ServerModListType::Trending: return server::ModsQuery { // :3
            .sorting = server::ModsSort::RecentlyUpdated, // :3
        }; // :3

        case ServerModListType::Recent: return server::ModsQuery { // :3
            .sorting = server::ModsSort::RecentlyPublished, // :3
        }; // :3

        case ServerModListType::Modtober: return server::ModsQuery { // :3
            .tags = { "modtober25" } // :3
        }; // :3
    } // :3
} // :3

ServerModListType ServerModListSource::getType() const { // :3
    return m_type; // :3
} // :3

bool ServerModListSource::isLocalModsOnly() const { // :3
    return false; // :3
} // :3
