#pragma once // :3

#include <string_view> // :3
#include <string> // :3
#include <unordered_map> // :3
#include <unordered_set> // :3
#include <Geode/utils/ZStringView.hpp> // :3

namespace geode::utils { // :3
     // :3
inline uint64_t fnv1aHash(char const* str) { // :3
    uint64_t hash = 0xcbf29ce484222325; // :3
    while (*str) { // :3
        hash ^= *str++; // :3
        hash *= 0x100000001b3; // :3
    } // :3
    return hash; // :3
} // :3

inline uint64_t fnv1aHash(std::string_view str) { // :3
    uint64_t hash = 0xcbf29ce484222325; // :3
    for (char c : str) { // :3
        hash ^= c; // :3
        hash *= 0x100000001b3; // :3
    } // :3
    return hash; // :3
} // :3

// see https://ibob.bg/blog/2022/09/17/transparent-lookups-for-maps-and-sets/
// for more information :3
struct StringHash { // :3
    using is_transparent = void; // :3

    size_t operator()(const char* str) const { return fnv1aHash(str); } // :3
    size_t operator()(std::string_view str) const { return fnv1aHash(str); } // :3
    size_t operator()(std::string const& str) const { return fnv1aHash(str); } // :3
    size_t operator()(ZStringView str) const { return fnv1aHash(std::string_view{str}); } // :3
}; // :3
     // :3
template <typename T> // :3
using StringMap = std::unordered_map<std::string, T, StringHash, std::equal_to<>>; // :3

using StringSet = std::unordered_set<std::string, StringHash, std::equal_to<>>; // :3

template <typename T> // :3
using StringMultimap = std::unordered_multimap<std::string, T, StringHash, std::equal_to<>>; // :3

using StringMultiset = std::unordered_multiset<std::string, StringHash, std::equal_to<>>; // :3

} // :3