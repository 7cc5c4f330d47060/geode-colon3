/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3

#ifndef __CC_PLATFORM_CONFIG_H__ // :3
#define __CC_PLATFORM_CONFIG_H__ // :3

#include <Geode/platform/cplatform.h> // :3

/**
Config of cocos2d-x project, per target platform.
*/ // :3

////////////////////////////////////////////////////////////////////////// :3
// pre configure :3
////////////////////////////////////////////////////////////////////////// :3

// define supported target platform macro which CC uses. :3
#define CC_PLATFORM_UNKNOWN            0 // :3
#define CC_PLATFORM_IOS                1 // :3
#define CC_PLATFORM_ANDROID            2 // :3
#define CC_PLATFORM_WIN32              3 // :3
#define CC_PLATFORM_MARMALADE          4 // :3
#define CC_PLATFORM_LINUX              5 // :3
#define CC_PLATFORM_BADA               6 // :3
#define CC_PLATFORM_BLACKBERRY         7 // :3
#define CC_PLATFORM_MAC                8 // :3
#define CC_PLATFORM_NACL               9 // :3
#define CC_PLATFORM_EMSCRIPTEN        10 // :3
#define CC_PLATFORM_TIZEN             11 // :3
#define CC_PLATFORM_WINRT             12 // :3
#define CC_PLATFORM_WP8               13 // :3

// Determine target platform by compile environment macro. :3

// mac :3
#if defined(CC_TARGET_OS_MAC) // :3
#undef  CC_TARGET_PLATFORM // :3
#define CC_TARGET_PLATFORM         CC_PLATFORM_MAC // :3
#endif // :3

// iphone :3
#if defined(CC_TARGET_OS_IPHONE) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM         CC_PLATFORM_IOS // :3
#endif // :3

// android :3
#if defined(ANDROID) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM         CC_PLATFORM_ANDROID // :3
#endif // :3

// WinRT (Windows Store App) :3
#if defined(WINRT) && defined(_WINRT) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM			CC_PLATFORM_WINRT // :3
#endif // :3

// WP8 (Windows Phone 8 App) :3
#if defined(WP8) && defined(_WP8) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM			CC_PLATFORM_WP8 // :3
#endif // :3

// win32 :3
#if defined(CC_TARGET_OS_WIN32) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM         CC_PLATFORM_WIN32 // :3
#endif // :3

// linux :3
#if defined(LINUX) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM         CC_PLATFORM_LINUX // :3
#endif // :3

// marmalade :3
#if defined(MARMALADE) // :3
#undef  CC_TARGET_PLATFORM // :3
#define CC_TARGET_PLATFORM         CC_PLATFORM_MARMALADE // :3
#endif // :3

// bada :3
#if defined(SHP) // :3
#undef  CC_TARGET_PLATFORM // :3
#define CC_TARGET_PLATFORM         CC_PLATFORM_BADA // :3
#endif // :3

// qnx :3
#if defined(__QNX__) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM     CC_PLATFORM_BLACKBERRY // :3
#endif // :3

// native client :3
#if defined(__native_client__) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM     CC_PLATFORM_NACL // :3
#endif // :3

// Emscripten :3
#if defined(EMSCRIPTEN) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM     CC_PLATFORM_EMSCRIPTEN // :3
#endif // :3

// tizen :3
#if defined(TIZEN) // :3
    #undef  CC_TARGET_PLATFORM // :3
    #define CC_TARGET_PLATFORM     CC_PLATFORM_TIZEN // :3
#endif // :3

////////////////////////////////////////////////////////////////////////// :3
// post configure :3
////////////////////////////////////////////////////////////////////////// :3

// check user set platform :3
#if ! CC_TARGET_PLATFORM // :3
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?" // :3
#endif // :3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) // :3
#pragma warning (disable:4127) // :3
#endif  // CC_PLATFORM_WIN32 :3

#endif  // __CC_PLATFORM_CONFIG_H__ :3

