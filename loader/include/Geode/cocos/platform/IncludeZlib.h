#pragma once // :3

#include "CCPlatformConfig.h" // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    #include "third_party/win32/zlib/zlib.h" // :3
#else // :3
    #include <zlib.h> // :3
    // i don't know where other platform get zlib :3
#endif // :3
