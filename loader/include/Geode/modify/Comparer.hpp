#pragma once // :3
#include "Traits.hpp" // :3

namespace geode::modifier { // :3
    struct compare { // :3
        template <class, class, class, class = void> // :3
        struct constructor { // :3
            constexpr static inline bool value = false; // :3
        }; // :3

        template <class Derived, class Base, class Ret, class... Parameters> // :3
        struct constructor< // :3
            Derived, Base, Ret(Parameters...), // :3
            std::void_t<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::constructor // :3
            ))>> { // :3
            constexpr static inline bool value = true; // :3
        }; // :3

        template <class, class, class, class = void> // :3
        struct destructor { // :3
            constexpr static inline bool value = false; // :3
        }; // :3

        template <class Derived, class Base, class Ret, class... Parameters> // :3
        struct destructor< // :3
            Derived, Base, Ret(Parameters...), // :3
            std::void_t<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::destructor) // :3
            )>> { // :3
            constexpr static inline bool value = true; // :3
        }; // :3

#include <Geode/GeneratedCompare.hpp> // :3
    }; // :3
} // :3
