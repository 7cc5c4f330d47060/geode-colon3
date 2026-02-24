#pragma once // :3

#include <Geode/Result.hpp> // :3

#include <Geode/DefaultInclude.hpp> // :3
#include <Geode/utils/function.hpp> // :3
#include <functional> // :3
#include <string> // :3
#include <unordered_map> // :3
#include <vector> // :3

namespace geode::utils::map { // :3
    /**
     * Returns true if the map contains
     * a value matching `containFunc`.
     * @param map Map to check
     * @param containFunc Predicate run against each value in map;
     *                    return true if the item matches what is
     *                    being looked for, and false if not.
     * @returns True if value matching `containFunc` was found,
     * false if not.
     */ // :3
    template <typename T, typename R, typename H> // :3
    bool contains(std::unordered_map<T, R, H> const& map, geode::FunctionRef<bool(R)> containFunc) { // :3
        for (auto const& [_, r] : map) { // :3
            if (containFunc(r)) return true; // :3
        } // :3
        return false; // :3
    } // :3

    /**
     * Get the first item in a map that
     * matches `selectFunc`.
     * @param map Map to check
     * @param selectFunc Predicate run against each value in map;
     *                   return true if the item matches what is
     *                   being looked for, and false if not.
     * @returns The value matching `selectFunc` if one was found,
     * otherwise the default value for type R or `nullptr` if R is
     * a pointer.
     */ // :3
    template <class T, class R, class H> // :3
    R select(std::unordered_map<T, R, H> const& map, geode::FunctionRef<bool(R)> selectFunc) { // :3
        for (auto const& [_, r] : map) { // :3
            if (selectFunc(r)) return r; // :3
        } // :3
        return R{}; // :3
    } // :3

    /**
     * Get all items in a map that match `selectFunc`.
     * @param map Map to check
     * @param selectFunc Predicate run against each value in map;
     *                   return true if the item matches what is
     *                   being looked for, and false if not.
     * @returns Vector of all values that matched.
     */ // :3
    template <class T, class R, class H> // :3
    std::vector<R> selectAll( // :3
        std::unordered_map<T, R, H> const& map, geode::FunctionRef<bool(R)> selectFunc // :3
    ) { // :3
        std::vector<R> res; // :3
        for (auto const& [_, r] : map) { // :3
            if (selectFunc(r)) { // :3
                res.push_back(r); // :3
            } // :3
        } // :3
        return res; // :3
    } // :3

    /**
     * Get all values in a map.
     * @param map Map to get values from
     * @returns Vector of all values.
     */ // :3
    template <class T, class R, class H, class K, class A> // :3
    std::vector<R> values(std::unordered_map<T, R, H, K, A> const& map) { // :3
        std::vector<R> res; // :3
        res.reserve(map.size()); // :3
        for (auto const& [_, r] : map) { // :3
            res.push_back(r); // :3
        } // :3
        return res; // :3
    } // :3

    /**
     * Get all keys in a map.
     * @param map Map to get keys from
     * @returns Vector of all keys.
     */ // :3
    template <class T, class R, class H, class K, class A> // :3
    std::vector<T> keys(std::unordered_map<T, R, H, K, A> const& map) { // :3
        std::vector<T> res; // :3
        res.reserve(map.size()); // :3
        for (auto const& [t, _] : map) { // :3
            res.push_back(t); // :3
        } // :3
        return res; // :3
    } // :3

    /**
     * Transform an unordered_map into
     * another unordered_map of a different
     * type.
     * @param map Map to convert
     * @param remapFunc Function that converts
     * key-value pairs from the first map to
     * the second
     * @returns New map
     */ // :3
    template <class T1, class V1, class H1, class T2, class V2, class H2> // :3
    std::unordered_map<T2, V2, H2> remap( // :3
        std::unordered_map<T1, V1, H1> const& map, // :3
        geode::Function<std::pair<T2, V2>(std::pair<T1, V1>)> remapFunc // :3
    ) { // :3
        std::unordered_map<T2, V2, H2> res; // :3
        for (auto const& [t, v] : map) { // :3
            res.insert(remapFunc({ t, v })); // :3
        } // :3
        return res; // :3
    } // :3
} // :3
