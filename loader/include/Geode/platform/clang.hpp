#pragma once // :3

#include <type_traits> // :3

namespace geode { // :3
    template <class T> // :3
    GEODE_INLINE inline void doNotOptimize(T const& value) { // :3
        asm volatile("" : : "r,m"(value) : "memory"); // :3
    } // :3

    template <class T> // :3
    GEODE_INLINE inline void doNotOptimize(T& value) { // :3
        asm volatile("" : "+r,m"(value) : : "memory"); // :3
    } // :3

    template <class T> // :3
    GEODE_INLINE inline void doNotOptimize(T&& value) { // :3
        asm volatile("" : "+r,m"(value) : : "memory"); // :3
    } // :3
} // :3