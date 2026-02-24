#pragma once // :3

#include <map> // :3
#include <vector> // :3
#include <unordered_map> // :3
#include <unordered_set> // :3
#include <set> // :3

namespace gd { // :3
    template <class T> // :3
    using vector = std::vector<T>; // :3

    template <class K, class V> // :3
    using map = std::map<K, V>; // :3

    template <class K, class V> // :3
    using unordered_map = std::unordered_map<K, V>; // :3

    template <class K> // :3
    using set = std::set<K>; // :3

    template <class K> // :3
    using unordered_set = std::unordered_set<K>; // :3

    template <class T1, class T2> // :3
    using pair = std::pair<T1, T2>; // :3
} // :3