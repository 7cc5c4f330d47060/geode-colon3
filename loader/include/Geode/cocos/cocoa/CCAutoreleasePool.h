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
#ifndef __AUTORELEASEPOOL_H__ // :3
#define __AUTORELEASEPOOL_H__ // :3

#include "CCObject.h" // :3
#include "CCArray.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup base_nodes
 * @{
 * @js NA
 * @lua NA
 */ // :3

class CC_DLL CCAutoreleasePool : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCArray*    m_pManagedObjectArray; // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCAutoreleasePool, CCObject) // :3

    CCAutoreleasePool(void); // :3
    ~CCAutoreleasePool(void); // :3

    void addObject(CCObject *pObject); // :3
    void removeObject(CCObject *pObject); // :3

    void clear(); // :3
}; // :3

/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCPoolManager // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    CCArray*    m_pReleasePoolStack; // :3
    CCAutoreleasePool*                    m_pCurReleasePool; // :3

    CCAutoreleasePool* getCurReleasePool(); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCPoolManager) // :3

    CCPoolManager(); // :3
    ~CCPoolManager(); // :3
    void finalize(); // :3
    void push(); // :3
    void pop(); // :3

    void removeObject(CCObject* pObject); // :3
    void addObject(CCObject* pObject); // :3

    static CCPoolManager* sharedPoolManager(); // :3
    static void purgePoolManager(); // :3

    friend class CCAutoreleasePool; // :3
}; // :3

// end of base_nodes group :3
/// @} :3

NS_CC_END // :3

#endif //__AUTORELEASEPOOL_H__ :3
