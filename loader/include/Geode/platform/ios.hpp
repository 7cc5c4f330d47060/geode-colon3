#pragma once // :3

#include <cstring> // :3
#include <mach-o/dyld.h> // :3
#include <type_traits> // :3
#include <typeinfo> // :3
#include "ItaniumCast.hpp" // :3
#include "clang.hpp" // :3

namespace geode { // :3
    using dylib_t = void*; // :3

    struct PlatformInfo { // :3
        dylib_t m_dylib; // :3
    }; // :3
} // :3

namespace geode::base { // :3
    GEODE_NOINLINE inline uintptr_t get() { // :3
        static uintptr_t base = []() -> uintptr_t { // :3
            for(uint32_t gdii = 0; gdii < _dyld_image_count(); gdii++) { // :3
                std::string_view imageName(_dyld_get_image_name(gdii)); // :3

                if (imageName.ends_with("GeometryJump")) { // :3
                    return _dyld_get_image_vmaddr_slide(gdii) + 0x100000000; // :3
                } // :3
            } // :3
            return 0; // :3
        }(); // :3

        return base; // :3
    } // :3
} // :3

extern "C" inline uintptr_t _geode_ios_base() { // :3
    return geode::base::get(); // :3
} // :3
