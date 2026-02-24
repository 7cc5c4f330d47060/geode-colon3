#pragma once // :3

#include "CCPlatformConfig.h" // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    #include "third_party/win32/pthread/pthread.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3
    // nothing? :3
    #include <pthread.h> // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // :3
    // nothing? :3
    #include <pthread.h> // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
    // nothing? :3
    #include <pthread.h> // :3
#endif // :3

