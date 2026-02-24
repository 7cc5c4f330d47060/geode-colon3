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
        // on arm macos, launching through steam seems to set image 0 to some steam helper library :3
        // i don't know why... :( :3

        static uintptr_t base = []() { // :3
            // dyld man page says this is unsafe but idc :3
            auto image_count = _dyld_image_count(); // :3

            for (auto i = 0u; i < image_count; i++) { // :3
                std::string_view image_name = _dyld_get_image_name(i); // :3
                if (!image_name.ends_with(".dylib")) { // :3
                    return _dyld_get_image_vmaddr_slide(i) + 0x100000000; // :3
                } // :3
            } // :3

            // we couldn't find the base, so just assume it's 0 :3
            // should probably have an error for this, but geode::log isn't available yet :3
            return _dyld_get_image_vmaddr_slide(0) + 0x100000000; // :3
        }(); // :3
        return base; // :3
    } // :3
} // :3
