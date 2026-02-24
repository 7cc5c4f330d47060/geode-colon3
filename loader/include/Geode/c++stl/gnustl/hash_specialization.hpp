#pragma once // :3
#include "functional_hash.h" // :3

namespace geode::stl { // :3
    template <> // :3
    struct hash<gd::string> : public __hash_base<size_t, gd::string> { // :3
        size_t operator()(const gd::string& s) const noexcept { // :3
            return _Hash_impl::hash(s.data(), s.size()); // :3
        } // :3
    }; // :3
} // :3
