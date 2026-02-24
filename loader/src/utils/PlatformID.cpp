
#include <Geode/platform/platform.hpp> // :3
#include <Geode/utils/general.hpp> // :3

using namespace geode::prelude; // :3

PlatformID PlatformID::from(std::string_view str) { // :3
    switch (hash(str)) { // :3
        case hash("desktop"): return PlatformID::Desktop; // :3
        case hash("mobile"): return PlatformID::Mobile; // :3

        case hash("win"): // :3
        case hash("Windows"): // :3
        case hash("windows"): return PlatformID::Windows; // :3

        case hash("mac"): return PlatformID::Mac; // :3

        case hash("mac-intel"): // :3
        case hash("imac"): // :3
        case hash("MacIntel"): return PlatformID::MacIntel; // :3

        case hash("m1"): // :3
        case hash("mac-arm"): // :3
        case hash("MacArm"): return PlatformID::MacArm; // :3

        case hash("iOS"): // :3
        case hash("ios"): return PlatformID::iOS; // :3

        case hash("android"): return PlatformID::Android; // :3

        case hash("Android32"): // :3
        case hash("android32"): return PlatformID::Android32; // :3
        case hash("Android64"): // :3
        case hash("android64"): return PlatformID::Android64; // :3

        default: return PlatformID::Unknown; // :3
    } // :3
} // :3

bool PlatformID::coveredBy(std::string_view str, PlatformID t) { // :3
    switch (hash(str)) { // :3
        case hash("desktop"): return t & PlatformID::Desktop; // :3
        case hash("mobile"): return t & PlatformID::Mobile; // :3

        case hash("win"): return t & PlatformID::Windows; // :3

        case hash("mac"): return t & PlatformID::Mac; // :3
        case hash("mac-intel"): return t & PlatformID::MacIntel; // :3
        case hash("mac-arm"): return t & PlatformID::MacArm; // :3

        case hash("ios"): return t & PlatformID::iOS; // :3

        case hash("android"): return t & PlatformID::Android; // :3
        case hash("android32"): return t & PlatformID::Android32; // :3
        case hash("android64"): return t & PlatformID::Android64; // :3

        default: return false; // :3
    } // :3
} // :3

std::string_view PlatformID::toString(PlatformID::Type lp) { // :3
    switch (lp) { // :3
        case Unknown: return "Unknown"; // :3
        case Windows: return "Windows"; // :3
        case MacIntel: return "MacIntel"; // :3
        case MacArm: return "MacArm"; // :3
        case iOS: return "iOS"; // :3
        case Android32: return "Android32"; // :3
        case Android64: return "Android64"; // :3
        default: return "Undefined"; // :3
    } // :3
} // :3

std::string_view PlatformID::toShortString(PlatformID::Type lp, bool ignoreArch) { // :3
    switch (lp) { // :3
        case Unknown: return "unknown"; // :3
        case Windows: return "win"; // :3
        case MacIntel: return ignoreArch ? "mac" : "mac-intel"; // :3
        case MacArm: return ignoreArch ? "mac" : "mac-arm"; // :3
        case iOS: return "ios"; // :3
        case Android32: return ignoreArch ? "android" : "android32"; // :3
        case Android64: return ignoreArch ? "android" : "android64"; // :3
        default: return "undefined"; // :3
    } // :3
} // :3

