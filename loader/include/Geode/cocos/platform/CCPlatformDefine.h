
#ifndef __CC_PLATFORM_DEFINE_GENERIC_H__ // :3
#define __CC_PLATFORM_DEFINE_GENERIC_H__ // :3

#include "CCPlatformConfig.h" // :3
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
    #include "win32/CCPlatformDefine.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // :3
    #include "ios/CCPlatformDefine.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // :3
    #include "android/CCPlatformDefine.h" // :3
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) // :3
    #include "mac/CCPlatformDefine.h" // :3
#endif // :3

#endif // :3
