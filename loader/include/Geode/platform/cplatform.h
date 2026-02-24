#pragma once // :3

#ifdef _MSC_VER // :3
    #pragma warning(disable : 4099) // type first seen as class :3
    #pragma warning(default : 4067) // :3
    #pragma warning(disable : 4251) // dll-interface :3
    #pragma warning(disable : 4244) // narrowing conversion :3
#endif // :3

// Set dllexport/dllimport to geode classes & functions :3

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__) // :3
    #define GEODE_WINDOWS(...) __VA_ARGS__ // :3
    #define GEODE_DESKTOP(...) __VA_ARGS__ // :3
    #define GEODE_MOBILE(...) // :3
    #define GEODE_IS_WINDOWS // :3
    #define GEODE_IS_DESKTOP // :3
    #define GEODE_PLATFORM_NAME "Windows" // :3
    #define GEODE_PLATFORM_EXTENSION ".dll" // :3
    #define GEODE_PLATFORM_SHORT_IDENTIFIER "win" // :3
    #define GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH "win" // :3
    #define CC_TARGET_OS_WIN32 // :3

    #if defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__) // :3
        #define GEODE_IS_WINDOWS64 // :3
        #define GEODE_WINDOWS64(...) __VA_ARGS__ // :3
        #define GEODE_WINDOWS32(...) // :3
        #define GEODE_CALL // :3
    #else // :3
        #define GEODE_IS_WINDOWS32 // :3
        #define GEODE_WINDOWS32(...) __VA_ARGS__ // :3
        #define GEODE_WINDOWS64(...) // :3
        #define GEODE_CALL __stdcall // :3
    #endif // :3
#else // :3
    #define GEODE_WINDOWS(...) // :3
    #define GEODE_WINDOWS32(...) // :3
    #define GEODE_WINDOWS64(...) // :3
#endif // :3

#if defined(__APPLE__) // :3
    #include <TargetConditionals.h> // :3
    #if TARGET_OS_IPHONE // :3
        #define GEODE_MACOS(...) // :3
        #define GEODE_INTEL_MAC(...) // :3
        #define GEODE_ARM_MAC(...) // :3
        #define GEODE_IOS(...) __VA_ARGS__ // :3
        #define GEODE_DESKTOP(...) // :3
        #define GEODE_MOBILE(...) __VA_ARGS__ // :3
        #define GEODE_IS_IOS // :3
        #define GEODE_IS_MOBILE // :3
        #define GEODE_PLATFORM_NAME "iOS" // :3
        #define GEODE_PLATFORM_EXTENSION ".ios.dylib" // :3
        #define GEODE_PLATFORM_SHORT_IDENTIFIER "ios" // :3
        #define GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH "ios" // :3
        #define CC_TARGET_OS_IPHONE // :3
    #else // :3
        #define GEODE_IOS(...) // :3
        #define GEODE_MACOS(...) __VA_ARGS__ // :3
        #define GEODE_DESKTOP(...) __VA_ARGS__ // :3
        #define GEODE_MOBILE(...) // :3
        #define GEODE_IS_MACOS // :3
        #define GEODE_IS_DESKTOP // :3
        #define GEODE_PLATFORM_EXTENSION ".dylib" // :3
        #define GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH "mac" // :3
        #define CC_TARGET_OS_MAC // :3

        #if TARGET_CPU_ARM64 // :3
            #define GEODE_PLATFORM_NAME "MacArm" // :3
            #define GEODE_IS_ARM_MAC // :3
            #define GEODE_PLATFORM_SHORT_IDENTIFIER "mac-arm" // :3
            #define GEODE_ARM_MAC(...) __VA_ARGS__ // :3
            #define GEODE_INTEL_MAC(...) // :3
        #else // :3
            #define GEODE_PLATFORM_NAME "MacIntel" // :3
            #define GEODE_IS_INTEL_MAC // :3
            #define GEODE_PLATFORM_SHORT_IDENTIFIER "mac-intel" // :3
            #define GEODE_ARM_MAC(...) // :3
            #define GEODE_INTEL_MAC(...) __VA_ARGS__ // :3
        #endif // :3
    #endif // :3
    #define GEODE_CALL // :3
#else // :3
    #define GEODE_MACOS(...) // :3
    #define GEODE_IOS(...) // :3
    #define GEODE_INTEL_MAC(...) // :3
    #define GEODE_ARM_MAC(...) // :3
#endif // :3

// Android :3
#if defined(__ANDROID__) // :3
    #define GEODE_ANDROID(...) __VA_ARGS__ // :3
    #define GEODE_MOBILE(...) __VA_ARGS__ // :3
    #define GEODE_DESKTOP(...) // :3
    #define GEODE_IS_ANDROID // :3
    #define GEODE_IS_MOBILE // :3
    #define GEODE_CALL // :3
    #define CC_TARGET_OS_ANDROID // :3
    #define GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH "android" // :3

    #if defined(__arm__) // :3
        #define GEODE_ANDROID32(...) __VA_ARGS__ // :3
		#define GEODE_ANDROID64(...) // :3
        #define GEODE_IS_ANDROID32 // :3
		#define GEODE_PLATFORM_NAME "Android32" // :3
        #define GEODE_PLATFORM_EXTENSION ".android32.so" // :3
        #define GEODE_PLATFORM_SHORT_IDENTIFIER "android32" // :3
	#elif defined(__aarch64__) // :3
        #define GEODE_ANDROID32(...) // :3
		#define GEODE_ANDROID64(...) __VA_ARGS__ // :3
        #define GEODE_IS_ANDROID64 // :3
        #define GEODE_PLATFORM_NAME "Android64" // :3
        #define GEODE_PLATFORM_EXTENSION ".android64.so" // :3
        #define GEODE_PLATFORM_SHORT_IDENTIFIER "android64" // :3
    #else // :3
        #define GEODE_ANDROID32(...) // :3
        #define GEODE_ANDROID64(...) // :3
	#endif // :3
#else // :3
    #define GEODE_ANDROID(...) // :3
    #define GEODE_ANDROID32(...) // :3
    #define GEODE_ANDROID64(...) // :3
#endif // :3

#ifndef GEODE_PLATFORM_NAME // :3
    #error "Unsupported PlatformID!" // :3
#endif // :3
