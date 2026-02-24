/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org

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

#ifndef __SCRIPT_SUPPORT_H__ // :3
#define __SCRIPT_SUPPORT_H__ // :3

#include "../platform/CCCommon.h" // :3
#include "../platform/CCAccelerometer.h" // :3
#include "../touch_dispatcher/CCTouch.h" // :3
#include "../cocoa/CCSet.h" // :3
#include <map> // :3
#include <string> // :3
#include <list> // :3

typedef struct lua_State lua_State; // :3

NS_CC_BEGIN // :3

class CCTimer; // :3
class CCLayer; // :3
class CCMenuItem; // :3
class CCNotificationCenter; // :3
class CCCallFunc; // :3
class CCAcceleration; // :3

enum ccScriptType { // :3
    kScriptTypeNone = 0, // :3
    kScriptTypeLua, // :3
    kScriptTypeJavascript // :3
}; // :3
/**
 * @js NA
 * @lua NA
 */ // :3
class CCScriptHandlerEntry : public CCObject // :3
{ // :3
public: // :3
    static CCScriptHandlerEntry* create(int nHandler); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCScriptHandlerEntry, CCObject) // :3
    ~CCScriptHandlerEntry(void); // :3

    int getHandler(void) { // :3
        return m_nHandler; // :3
    } // :3

    int getEntryId(void) { // :3
        return m_nEntryId; // :3
    } // :3

protected: // :3
    CCScriptHandlerEntry(int nHandler) // :3
    : m_nHandler(nHandler) // :3
    { // :3
        static int newEntryId = 0; // :3
        newEntryId++; // :3
        m_nEntryId = newEntryId; // :3
    } // :3
public: // :3
    int m_nHandler; // :3
    int m_nEntryId; // :3
}; // :3

/**
 * @addtogroup script_support
 * @{
 * @js NA
 * @lua NA
 */ // :3

class CCSchedulerScriptHandlerEntry : public CCScriptHandlerEntry // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    // nHandler return by tolua_ref_function(), called from LuaCocos2d.cpp :3
    static CCSchedulerScriptHandlerEntry* create(int nHandler, float fInterval, bool bPaused); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSchedulerScriptHandlerEntry, CCScriptHandlerEntry) // :3
    ~CCSchedulerScriptHandlerEntry(void); // :3

    cocos2d::CCTimer* getTimer(void) { // :3
        return m_pTimer; // :3
    } // :3

    bool isPaused(void) { // :3
        return m_bPaused; // :3
    } // :3

    void markedForDeletion(void) { // :3
        m_bMarkedForDeletion = true; // :3
    } // :3

    bool isMarkedForDeletion(void) { // :3
        return m_bMarkedForDeletion; // :3
    } // :3

private: // :3
    CCSchedulerScriptHandlerEntry(int nHandler) // :3
    : CCScriptHandlerEntry(nHandler) // :3
    , m_pTimer(NULL) // :3
    , m_bPaused(false) // :3
    , m_bMarkedForDeletion(false) // :3
    { // :3
    } // :3
    bool init(float fInterval, bool bPaused); // :3
public: // :3
    cocos2d::CCTimer*   m_pTimer; // :3
    bool                m_bPaused; // :3
    bool                m_bMarkedForDeletion; // :3
}; // :3


/**
 * @js NA
 * @lua NA
 */ // :3
class CCTouchScriptHandlerEntry : public CCScriptHandlerEntry // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    static CCTouchScriptHandlerEntry* create(int nHandler, bool bIsMultiTouches, int nPriority, bool bSwallowsTouches); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTouchScriptHandlerEntry, CCScriptHandlerEntry) // :3
    ~CCTouchScriptHandlerEntry(void); // :3

    bool isMultiTouches(void) { // :3
        return m_bIsMultiTouches; // :3
    } // :3

    int getPriority(void) { // :3
        return m_nPriority; // :3
    } // :3

    bool getSwallowsTouches(void) { // :3
        return m_bSwallowsTouches; // :3
    } // :3

private: // :3
    CCTouchScriptHandlerEntry(int nHandler) // :3
    : CCScriptHandlerEntry(nHandler) // :3
    , m_bIsMultiTouches(false) // :3
    , m_nPriority(0) // :3
    , m_bSwallowsTouches(false) // :3
    { // :3
    } // :3
    bool init(bool bIsMultiTouches, int nPriority, bool bSwallowsTouches); // :3
public: // :3
    bool    m_bIsMultiTouches; // :3
    int     m_nPriority; // :3
    bool    m_bSwallowsTouches; // :3
}; // :3


// Don't make CCScriptEngineProtocol inherits from CCObject since setScriptEngine is invoked only once in AppDelegate.cpp, :3
// It will affect the lifecycle of ScriptCore instance, the autorelease pool will be destroyed before destructing ScriptCore. :3
// So a crash will appear on Win32 if you click the close button. :3
/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCScriptEngineProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual ~CCScriptEngineProtocol() {}; // :3

    /** Get script type */
    virtual ccScriptType getScriptType() { return kScriptTypeNone; };

    /** Remove script object. */
    virtual void removeScriptObjectByCCObject(CCObject* pObj) = 0;

    /** Remove script function handler, only CCLuaEngine class need to implement this function. */
    virtual void removeScriptHandler(int nHandler) {};

    /** Reallocate script function handler, only CCLuaEngine class need to implement this function. */
    virtual int reallocateScriptHandler(int nHandler) { return -1;}

    /**
     @brief Execute script code contained in the given string.
     @param codes holding the valid script code that should be executed.
     @return 0 if the string is executed correctly.
     @return other if the string is executed wrongly.
     */ // :3
    virtual int executeString(const char* codes) = 0; // :3

    /**
     @brief Execute a script file.
     @param filename String object holding the filename of the script file that is to be executed
     */ // :3
    virtual int executeScriptFile(const char* filename) = 0; // :3

    /**
     @brief Execute a scripted global function.
     @brief The function should not take any parameters and should return an integer.
     @param functionName String object holding the name of the function, in the global script environment, that is to be executed.
     @return The integer value returned from the script function.
     */ // :3
    virtual int executeGlobalFunction(const char* functionName) = 0; // :3

    /**
     @brief Execute a node event function
     @param pNode which node produce this event
     @param nAction kCCNodeOnEnter,kCCNodeOnExit,kCCMenuItemActivated,kCCNodeOnEnterTransitionDidFinish,kCCNodeOnExitTransitionDidStart
     @return The integer value returned from the script function.
     */ // :3
    virtual int executeNodeEvent(CCNode* pNode, int nAction) = 0; // :3

    virtual int executeMenuItemEvent(CCMenuItem* pMenuItem) = 0; // :3
    /** Execute a notification event function */
    virtual int executeNotificationEvent(CCNotificationCenter* pNotificationCenter, const char* pszName) = 0;

    /** execute a callfun event */
    virtual int executeCallFuncActionEvent(CCCallFunc* pAction, CCObject* pTarget = NULL) = 0;
    /** execute a schedule function */
    virtual int executeSchedule(int nHandler, float dt, CCNode* pNode = NULL) = 0;

    /** functions for executing touch event */
    virtual int executeLayerTouchesEvent(CCLayer* pLayer, int eventType, CCSet *pTouches) = 0;
    virtual int executeLayerTouchEvent(CCLayer* pLayer, int eventType, CCTouch *pTouch) = 0;

    /** functions for keypad event */
    virtual int executeLayerKeypadEvent(CCLayer* pLayer, int eventType) = 0;

    /** execute a accelerometer event */
    virtual int executeAccelerometerEvent(CCLayer* pLayer, CCAcceleration* pAccelerationValue) = 0;

    /** function for common event */
    virtual int executeEvent(int nHandler, const char* pEventName, CCObject* pEventSource = NULL, const char* pEventSourceClassName = NULL) = 0;

    /** function for c++ call back lua funtion */
    virtual int executeEventWithArgs(int nHandler, CCArray* pArgs) { return 0; }

    /** called by CCAssert to allow scripting engine to handle failed assertions
     * @return true if the assert was handled by the script engine, false otherwise.
     */ // :3
    virtual bool handleAssert(const char *msg) = 0; // :3

    /**
     *
     */ // :3
    enum ConfigType // :3
    { // :3
        NONE, // :3
        COCOSTUDIO, // :3
    }; // :3
    virtual bool parseConfig(ConfigType type, const gd::string& str) = 0; // :3
}; // :3

/**
 CCScriptEngineManager is a singleton which holds an object instance of CCScriptEngineProtocl
 It helps cocos2d-x and the user code to find back LuaEngine object
 @since v0.99.5-x-0.8.5
 @js NA
 @lua NA
 */ // :3
class CC_DLL CCScriptEngineManager // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCScriptEngineManager) // :3
    ~CCScriptEngineManager(void); // :3

    CCScriptEngineProtocol* getScriptEngine(void) { // :3
        return m_pScriptEngine; // :3
    } // :3
    void setScriptEngine(CCScriptEngineProtocol *pScriptEngine); // :3
    void removeScriptEngine(void); // :3

    static CCScriptEngineManager* sharedManager(void); // :3
    static void purgeSharedManager(void); // :3

private: // :3
    CCScriptEngineManager(void) // :3
    : m_pScriptEngine(NULL) // :3
    { // :3
    } // :3
public: // :3
    CCScriptEngineProtocol *m_pScriptEngine; // :3
}; // :3

// end of script_support group :3
/// @} :3

NS_CC_END // :3

#endif // __SCRIPT_SUPPORT_H__ :3
