/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __CC_PLATFORM_THREAD_H__ // :3
#define __CC_PLATFORM_THREAD_H__ // :3

#include "CCCommon.h" // :3
#include "CCPlatformMacros.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup platform
 * @{
 */ // :3

/* On iOS, should create autorelease pool when create a new thread
 * and release it when the thread end.
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCThread // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCThread) // :3
    CCThread() : m_pAutoreasePool(0) {} // :3
    ~CCThread(); // :3

    void createAutoreleasePool(); // :3

private: // :3
    void *m_pAutoreasePool; // :3
}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_PLATFORM_THREAD_H__ :3
