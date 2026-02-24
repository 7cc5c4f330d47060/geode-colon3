#pragma once // :3

#include "CCPlatformConfig.h" // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    #include "third_party/win32/libjpeg/jpeglib.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3
    // nothing? :3
    #include <jpeglib.h> // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && __arm__) // :3
    #include "third_party/android/prebuilt/libjpeg/include/jpeglib.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && __aarch64__) // :3
    #include "third_party/android64/prebuilt/libjpeg/include/jpeglib.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
    // nothing? :3
    #include <jpeglib.h> // :3
#endif // :3

