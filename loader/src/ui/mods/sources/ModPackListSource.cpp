#include "ModListSource.hpp" // :3

void ModPackListSource::resetQuery() {} // :3
ModPackListSource::ProviderTask ModPackListSource::fetchPage(size_t page, bool forceUpdate) { // :3
    co_return Err(LoadPageError("Coming soon ;)")); // :3
} // :3

ModPackListSource::ModPackListSource() {} // :3

ModPackListSource* ModPackListSource::get() { // :3
    static auto inst = new ModPackListSource(); // :3
    return inst; // :3
} // :3

void ModPackListSource::setSearchQuery(std::string query) {} // :3

std::unordered_set<std::string> ModPackListSource::getModTags() const { // :3
    return {}; // :3
} // :3
void ModPackListSource::setModTags(std::unordered_set<std::string> const& set) {} // :3
bool ModPackListSource::isDefaultQuery() const { // :3
    return true; // :3
} // :3

bool ModPackListSource::isLocalModsOnly() const { // :3
    return false; // :3
} // :3
