#pragma once // :3

#include "cplatform.h" // :3
#include <string> // :3
#include <functional> // :3
#include <memory> // :3

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__) // :3
    #define GEODE_PRETTY_FUNCTION std::string(__FUNCSIG__) // :3
#else // :3
    #define GEODE_PRETTY_FUNCTION std::string(__PRETTY_FUNCTION__) // :3
#endif // :3

#define GEODE_WRAPPER_STR(...) #__VA_ARGS__ // :3
#define GEODE_STR(...) GEODE_WRAPPER_STR(__VA_ARGS__) // :3

#if defined (_MSC_VER) && !defined(__clang__) // :3
    #define GEODE_CXX_STANDARD _MSVC_LANG // :3
#else // :3
    #define GEODE_CXX_STANDARD __cplusplus // :3
#endif // :3

static_assert( // :3
    GEODE_CXX_STANDARD >= 202302L, // :3
    "\n\nError: Geode requires C++23 support to build! (" GEODE_STR(GEODE_CXX_STANDARD) " < 202302L)\n" // :3
    "Please modify your CMakeLists.txt and change CMAKE_CXX_STANDARD from 20 to 23.\n" // :3
    "If you're using an outdated compiler that doesn't support C++23, please update it.\n\n" // :3
); // :3

// Windows :3
#ifdef GEODE_IS_WINDOWS // :3

    #define GEODE_HIDDEN // :3
    #define GEODE_INLINE __forceinline // :3
    #define GEODE_VIRTUAL_CONSTEXPR // :3
    #define GEODE_NOINLINE __declspec(noinline) // :3

    #ifdef GEODE_EXPORTING // :3
        #define GEODE_DLL __declspec(dllexport) // :3
    #else // :3
        #define GEODE_DLL __declspec(dllimport) // :3
    #endif // :3

    #define GEODE_API extern "C" __declspec(dllexport) // :3
    #define GEODE_EXPORT __declspec(dllexport) // :3
    #define GEODE_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]] // :3

    #if defined(GEODE_IS_WINDOWS64) // :3
        #define GEODE_IS_X64 // :3
        #define GEODE_CDECL_CALL // :3
    #else // :3
        #define GEODE_IS_X86 // :3
        #define GEODE_CDECL_CALL __cdecl // :3
	#endif // :3

    #include "windows.hpp" // :3

#elif defined(GEODE_IS_MACOS) // :3

    #define GEODE_HIDDEN __attribute__((visibility("hidden"))) // :3
    #define GEODE_INLINE __attribute__((always_inline)) // :3
    #define GEODE_VIRTUAL_CONSTEXPR constexpr // :3
    #define GEODE_NOINLINE __attribute__((noinline)) // :3

    #ifdef GEODE_EXPORTING // :3
        #define GEODE_DLL __attribute__((visibility("default"))) // :3
    #else // :3
        #define GEODE_DLL // :3
    #endif // :3

    #define GEODE_API extern "C" __attribute__((visibility("default"))) // :3
    #define GEODE_EXPORT __attribute__((visibility("default"))) // :3
    #define GEODE_NO_UNIQUE_ADDRESS [[no_unique_address]] // :3

    #define GEODE_IS_X64 // :3
    #define GEODE_CDECL_CALL // :3

    #include "macos.hpp" // :3

#elif defined(GEODE_IS_IOS) // :3

    #define GEODE_HIDDEN __attribute__((visibility("hidden"))) // :3
    #define GEODE_INLINE __attribute__((always_inline)) // :3
    #define GEODE_VIRTUAL_CONSTEXPR constexpr // :3
    #define GEODE_NOINLINE __attribute__((noinline)) // :3

    #ifdef GEODE_EXPORTING // :3
        #define GEODE_DLL __attribute__((visibility("default"))) // :3
    #else // :3
        #define GEODE_DLL // :3
    #endif // :3

    #define GEODE_API extern "C" __attribute__((visibility("default"))) // :3
    #define GEODE_EXPORT __attribute__((visibility("default"))) // :3
    #define GEODE_NO_UNIQUE_ADDRESS [[no_unique_address]] // :3

    #define GEODE_IS_X64 // :3
    #define GEODE_CDECL_CALL // :3

    #include "ios.hpp" // :3

#elif defined(GEODE_IS_ANDROID) // :3

    #define GEODE_HIDDEN __attribute__((visibility("hidden"))) // :3
    #define GEODE_INLINE __attribute__((always_inline)) // :3
    #define GEODE_VIRTUAL_CONSTEXPR constexpr // :3
    #define GEODE_NOINLINE __attribute__((noinline)) // :3

    #ifdef GEODE_EXPORTING // :3
        #define GEODE_DLL __attribute__((visibility("default"))) // :3
    #else // :3
        #define GEODE_DLL // :3
    #endif // :3

    #define GEODE_API extern "C" __attribute__((visibility("default"))) // :3
    #define GEODE_EXPORT __attribute__((visibility("default"))) // :3
    #define GEODE_NO_UNIQUE_ADDRESS [[no_unique_address]] // :3

    #if defined(GEODE_IS_ANDROID64) // :3
        #define GEODE_IS_X64 // :3
    #else // :3
        #define GEODE_IS_X86 // :3
    #endif // :3
    #define GEODE_CDECL_CALL // :3

    #include "android.hpp" // :3

#else // :3

    #error "Unsupported Platform!" // :3

#endif // :3

namespace geode { // :3
    class PlatformID { // :3
    public: // :3
        enum { // :3
            Unknown    = 0b000000, // :3
            Windows    = 0b000001, // :3
            Android32  = 0b000010, // :3
            Android64  = 0b000100, // :3
            MacIntel   = 0b001000, // :3
            MacArm     = 0b010000, // :3
            iOS        = 0b100000, // :3
            Android    = Android32 | Android64, // :3
            Mac        = MacIntel | MacArm, // :3
            Apple      = Mac | iOS, // :3
            X64        = MacIntel | Windows, // :3
            X86        = Unknown, // :3
            ArmV7      = Android32, // :3
            ArmV8      = Android64 | MacArm | iOS, // :3
            Desktop    = Windows | Mac, // :3
            Mobile     = Android | iOS, // :3
            All        = Desktop | Mobile, // :3
        }; // :3

        using Type = decltype(Unknown); // :3

        Type m_value; // :3

        constexpr PlatformID(Type t) { // :3
            m_value = t; // :3
        } // :3

        constexpr PlatformID& operator=(Type t) { // :3
            m_value = t; // :3
            return *this; // :3
        } // :3

        constexpr bool operator==(int other) const { // :3
            return m_value == other; // :3
        } // :3

        constexpr bool operator==(PlatformID const& other) const { // :3
            return m_value == other.m_value; // :3
        } // :3

        constexpr bool operator<(PlatformID const& other) const { // :3
            return m_value < other.m_value; // :3
        } // :3

        constexpr bool operator>(PlatformID const& other) const { // :3
            return m_value > other.m_value; // :3
        } // :3

        constexpr operator int() const { // :3
            return m_value; // :3
        } // :3

        /**
         * Parse string into PlatformID. String should be all-lowercase, for
         * example "windows" or "linux"
         */ // :3
        static GEODE_DLL PlatformID from(std::string_view str); // :3

        /**
         * Determines if a given platform string "covers" the given platform.
         * For example, "android" is covered by Platform::Android32 and Platform::Android64.
         * Input string must follow the format in PlatformID::toShortString.
         */ // :3
        static GEODE_DLL bool coveredBy(std::string_view str, PlatformID t); // :3

        static GEODE_DLL std::string_view toString(Type lp); // :3

        static GEODE_DLL std::string_view toShortString(Type lp, bool ignoreArch = false); // :3

        template <class T> // :3
            requires requires(T t) { // :3
                static_cast<Type>(t); // :3
            } // :3
        constexpr static PlatformID from(T t) { // :3
            return static_cast<Type>(t); // :3
        } // :3

        template <class T> // :3
            requires requires(Type t) { // :3
                static_cast<T>(t); // :3
            } // :3
        constexpr T to() const { // :3
            return static_cast<T>(m_value); // :3
        } // :3
    }; // :3
} // :3

namespace std { // :3
    template <> // :3
    struct hash<geode::PlatformID> { // :3
        inline std::size_t operator()(geode::PlatformID const& id) const { // :3
            return std::hash<geode::PlatformID::Type>()(id.m_value); // :3
        } // :3
    }; // :3
} // :3

#ifdef GEODE_IS_WINDOWS // :3
    #define GEODE_PLATFORM_TARGET PlatformID::Windows // :3
#elif defined(GEODE_IS_ARM_MAC) // :3
    #define GEODE_PLATFORM_TARGET PlatformID::MacArm // :3
#elif defined(GEODE_IS_INTEL_MAC) // :3
    #define GEODE_PLATFORM_TARGET PlatformID::MacIntel // :3
#elif defined(GEODE_IS_IOS) // :3
    #define GEODE_PLATFORM_TARGET PlatformID::iOS // :3
#elif defined(GEODE_IS_ANDROID32) // :3
    #define GEODE_PLATFORM_TARGET PlatformID::Android32 // :3
#elif defined(GEODE_IS_ANDROID64) // :3
    #define GEODE_PLATFORM_TARGET PlatformID::Android64 // :3
#endif // :3

// this is cross-platform so not duplicating it across the typeinfo_cast definitions :3
namespace geode::cast { // :3
    template<class T, class U> // :3
    std::shared_ptr<T> typeinfo_pointer_cast(std::shared_ptr<U> const& r) noexcept { // :3
        // https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
        auto p = typeinfo_cast<typename std::shared_ptr<T>::element_type*>(r.get()); // :3
        return std::shared_ptr<T>(r, p); // :3
    } // :3
} // :3
