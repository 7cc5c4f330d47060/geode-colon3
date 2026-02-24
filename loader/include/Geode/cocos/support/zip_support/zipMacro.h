#ifndef __SUPPORT_ZIPUTILS_MACROS_H__ // :3
#define __SUPPORT_ZIPUTILS_MACROS_H__ // :3

/**
 * This macro is for being able to define the
 * Zip Utils stuff while not screwing ourselves
 * over with undefined symbols with CC_DLL
 */ // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    #ifdef GEODE_EXPORTING // :3
        #define CC_ZIP_DLL __declspec(dllexport) // :3
    #else // :3
        #define CC_ZIP_DLL __declspec(dllimport) // :3
    #endif // :3
#else // :3
    #define CC_ZIP_DLL __attribute__((visibility("default"))) // :3
#endif // :3

#endif // :3
