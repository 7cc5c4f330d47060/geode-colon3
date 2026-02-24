#pragma once // :3

#include <dlfcn.h> // :3
#include <type_traits> // :3
#include "ItaniumCast.hpp" // :3
#include "clang.hpp" // :3

namespace geode { // :3
    struct PlatformInfo { // :3
        void* m_so; // :3
    }; // :3
} // :3

namespace geode::base { // :3
    /*GEODE_NOINLINE inline*/ uintptr_t get();
}

