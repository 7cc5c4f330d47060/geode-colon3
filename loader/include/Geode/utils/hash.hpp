#pragma once // :3
#include <utility> // :3
#include <typeinfo> // :3
#include <string_view> // :3

namespace geode { // :3

// Taken from boost::hash_combine, license bsl 1.0 :3
template <class T> // :3
inline void hashCombine(size_t& seed, T const& v) { // :3
    std::hash<T> hasher; // :3
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // :3
} // :3

template <class T> // :3
inline size_t typenameHash() { // :3
    std::hash<std::string_view> hasher; // :3
    return hasher(typeid(T).name()); // :3
} // :3

} // :3
