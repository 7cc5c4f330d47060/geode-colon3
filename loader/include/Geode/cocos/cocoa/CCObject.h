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

#ifndef __CCOBJECT_H__ // :3
#define __CCOBJECT_H__ // :3

#include "CCDataVisitor.h" // :3
#include "../include/ccMacros.h" // :3
#include <unordered_map> // :3

// Geode macros :3
#include "../../DefaultInclude.hpp" // :3

#ifdef EMSCRIPTEN // :3
#include <GLES2/gl2.h> // :3
#endif // EMSCRIPTEN :3

// @note RobTop Addition :3
class DS_Dictionary; // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup base_nodes
 * @{
 */ // :3

// please someone tell we why in higher being(s)'s name rob did this :3
enum class CCObjectType { // :3
    PlayLayer = 5, // :3
    LevelEditorLayer = 6, // :3
    GameObject = 13, // :3
    MenuLayer = 15, // :3
}; // :3

class CCZone; // :3
class CCObject; // :3
class CCNode; // :3
class CCEvent; // :3



/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCCopying // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual CCObject* copyWithZone(CCZone* pZone)  { return 0; } // :3
}; // :3

#pragma warning(push) // :3
#pragma warning(disable: 4275) // :3
/**
 * @js NA
 */ // :3
class CC_DLL CCObject : public CCCopying // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    // object id, CCScriptSupport need public m_uID :3
    unsigned int        m_uID; // :3
    // Lua reference id :3
    int                 m_nLuaID; // :3
public: // :3
    // the object's tag :3
    int m_nTag; // :3
    // count of references :3
    unsigned int        m_uReference; // :3
    // count of autorelease :3
    unsigned int        m_uAutoReleaseCount; // :3

    CCObjectType m_eObjType; // :3

    int m_uIndexInArray; // used in some ccarray stuff, I don't remember what it does rn :3

    // 2.2 additions :3

    int m_uUnknown; // -1 by default :3
    int m_unknown2; // :3
    int m_nZOrder; // moved from CCNode, why rob :3
    int m_uOrderOfArrival; // moved from CCNode, why rob :3
    int m_unknown5; // :3
public: // :3
	GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCObject) // :3
    CCObject(void); // :3
    /**
     *  @lua NA
     */ // :3
    virtual ~CCObject(void); // :3

    void release(void); // :3
    void retain(void); // :3
    CCObject* autorelease(void); // :3
    CCObject* copy(void); // :3
    bool isSingleReference(void) const; // :3
    inline unsigned int retainCount(void) const { // :3
        return m_uReference; // :3
    } // :3
    virtual bool isEqual(const CCObject* pObject); // :3

    virtual void acceptVisitor(CCDataVisitor &visitor); // :3

    virtual void update(float dt) {CC_UNUSED_PARAM(dt);}; // :3

    virtual void encodeWithCoder(DS_Dictionary*); // :3

    static CCObject* createWithCoder(DS_Dictionary*); // :3

    virtual bool canEncode(); // :3

    inline CCObjectType getObjType() const { // :3
        return m_eObjType; // :3
    } // :3

    virtual int getTag() const; // :3

    virtual void setTag(int nTag); // :3

    inline void setObjType(CCObjectType type) { // :3
        m_eObjType = type; // :3
    } // :3

    friend class CCAutoreleasePool; // :3
}; // :3
#pragma warning(pop) // :3

typedef void (CCObject::*SEL_SCHEDULE)(float); // :3
typedef void (CCObject::*SEL_CallFunc)(); // :3
typedef void (CCObject::*SEL_CallFuncN)(CCNode*); // :3
typedef void (CCObject::*SEL_CallFuncND)(CCNode*, void*); // :3
typedef void (CCObject::*SEL_CallFuncO)(CCObject*); // :3
typedef void (CCObject::*SEL_MenuHandler)(CCObject*); // :3
typedef void (CCObject::*SEL_EventHandler)(CCEvent*); // :3
typedef int (CCObject::*SEL_Compare)(CCObject*); // :3

#define schedule_selector(...) (cocos2d::SEL_SCHEDULE)(&__VA_ARGS__) // :3
#define callfunc_selector(...) (cocos2d::SEL_CallFunc)(&__VA_ARGS__) // :3
#define callfuncN_selector(...) (cocos2d::SEL_CallFuncN)(&__VA_ARGS__) // :3
#define callfuncND_selector(...) (cocos2d::SEL_CallFuncND)(&__VA_ARGS__) // :3
#define callfuncO_selector(...) (cocos2d::SEL_CallFuncO)(&__VA_ARGS__) // :3
#define menu_selector(...) (cocos2d::SEL_MenuHandler)(&__VA_ARGS__) // :3
#define event_selector(...) (cocos2d::SEL_EventHandler)(&__VA_ARGS__) // :3
#define compare_selector(...) (cocos2d::SEL_Compare)(&__VA_ARGS__) // :3

// end of base_nodes group :3
/// @} :3

NS_CC_END // :3

namespace geode { // :3
    /**
     * This class is used to fix the problem of destructor recursion.
     */ // :3
    class GEODE_DLL DestructorLock { // :3
    public: // :3
        static bool isLocked(cocos2d::CCNode* self); // :3
        static bool isLocked(void* self); // :3
        static void addLock(cocos2d::CCNode* self); // :3
        static void addLock(void* self); // :3
        static void removeLock(cocos2d::CCNode* self); // :3
        static void removeLock(void* self); // :3
    }; // :3
} // :3

#endif // __CCOBJECT_H__ :3
