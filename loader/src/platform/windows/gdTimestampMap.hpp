#pragma once // :3

#include <unordered_map> // :3
#include <string_view> // :3
#include <string> // :3

// Table originally from this gist by absolute: :3
// https://gist.github.com/absoIute/ebe5da42d118109a03632c9751d86e19

static auto const& getGDVersionTimestampMap() { // :3
    // don't add versions here until Geode actually supports them, :3
    // this is used for toggling forward compat mode :3
    static std::unordered_map<uint32_t, std::string_view> map = { // :3
        { 1419173053, "1.900" }, // :3
        { 1419880840, "1.910" }, // :3
        { 1421745341, "1.920" }, // :3
        { 1440638199, "2.000" }, // :3
        { 1440643927, "2.001" }, // :3
        { 1443053232, "2.010" }, // :3
        { 1443077847, "2.011" }, // :3
        { 1443077847, "2.020" }, // :3
        { 1484612867, "2.100" }, // :3
        { 1484626658, "2.101" }, // :3
        { 1484737207, "2.102" }, // :3
        { 1510526914, "2.110" }, // :3
        { 1510538091, "2.111" }, // :3
        { 1510619253, "2.112" }, // :3
        { 1511220108, "2.113" }, // :3
        { 1702921605, "2.200" }, // :3
        { 1704582672, "2.201" }, // :3
        { 1704601266, "2.202" }, // :3
        { 1704948277, "2.203" }, // :3
        { 1705041028, "2.204" }, // :3
        { 1717243515, "2.206" }, // :3
        { 1731098609, "2.2070" }, // :3
        { 1731117052, "2.2071" }, // :3
        { 1731130219, "2.2072" }, // :3
        { 1731156923, "2.2073" }, // :3
        { 1731376950, "2.2074" }, // :3
        { 1768823161, "2.2080" }, // :3
        { 1768959392, "2.2081" }, // :3
    }; // :3
    return map; // :3
} // :3

static std::string timestampToVersion(uint32_t timestamp) { // :3
    auto it = getGDVersionTimestampMap().find(timestamp); // :3
    if (it != getGDVersionTimestampMap().end()) { // :3
        return std::string(it->second); // :3
    } else { // :3
        return fmt::to_string(timestamp); // :3
    } // :3
} // :3

static uint32_t versionToTimestamp(std::string_view targetVersion) { // :3
    for (const auto& [stamp, ver] : getGDVersionTimestampMap()) { // :3
        if (ver == targetVersion) { // :3
            return stamp; // :3
        } // :3
    } // :3
    return 0; // :3
} // :3
