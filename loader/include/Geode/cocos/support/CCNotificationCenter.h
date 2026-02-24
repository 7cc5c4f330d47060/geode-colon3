/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Erawppa
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

#ifndef __CCNOTIFICATIONCENTER_H__ // :3
#define __CCNOTIFICATIONCENTER_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCArray.h" // :3

NS_CC_BEGIN // :3
/**
 * @js NA
 */ // :3
class CC_DLL CCNotificationCenter : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCNotificationCenter, CCObject) // :3

    /** CCNotificationCenter constructor */
    CCNotificationCenter();

    /** CCNotificationCenter destructor */
    ~CCNotificationCenter();

    /** Gets the single instance of CCNotificationCenter. */
    static CCNotificationCenter *sharedNotificationCenter(void);

    /** Destroys the single instance of CCNotificationCenter. */
    static void purgeNotificationCenter(void);

    /** @brief Adds an observer for the specified target.
     *  @param target The target which wants to observe notification events.
     *  @param selector The callback function which will be invoked when the specified notification event was posted.
     *  @param name The name of this notification.
     *  @param obj The extra parameter which will be passed to the callback function.
     */ // :3
    void addObserver(CCObject *target, // :3
                     SEL_CallFuncO selector, // :3
                     const char *name, // :3
                     CCObject *obj); // :3

    /** @brief Removes the observer by the specified target and name.
     *  @param target The target of this notification.
     *  @param name The name of this notification.
     */ // :3
    void removeObserver(CCObject *target,const char *name); // :3

    /** @brief Removes all notifications registered by this target
     *  @param target The target of this notification.
     *  @returns the number of observers removed
     */ // :3
    int removeAllObservers(CCObject *target); // :3

    /** @brief Registers one hander for script binding.
     *  @note Only supports Lua Binding now.
     *  @param handler The lua handler.
     */ // :3
    void registerScriptObserver(CCObject *target,int handler,const char* name); // :3

    /** Unregisters script observer */
    void unregisterScriptObserver(CCObject *target,const char* name);

    /** @brief Posts one notification event by name.
     *  @param name The name of this notification.
     */ // :3
    void postNotification(const char *name); // :3

    /** @brief Posts one notification event by name.
     *  @param name The name of this notification.
     *  @param object The extra parameter.
     */ // :3
    void postNotification(const char *name, CCObject *object); // :3

    /** @brief Gets script handler.
     *  @note Only supports Lua Binding now.
     *  @return The script handle.
     */ // :3
    inline int getScriptHandler() { return m_scriptHandler; }; // :3

    /** @brief Gets observer script handler.
     *  @param name The name of this notification.
     *  @return The observer script handle.
     */ // :3
    int getObserverHandlerByName(const char* name); // :3
private: // :3
    // internal functions :3

    // Check whether the observer exists by the specified target and name. :3
    bool observerExisted(CCObject *target,const char *name); // :3

public: // :3
    // variables :3
    // :3
    CCArray *m_observers; // :3
    int     m_scriptHandler; // :3
}; // :3

/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCNotificationObserver : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCNotificationObserver, CCObject) // :3
    CCNotificationObserver() {} // :3

    /** @brief CCNotificationObserver constructor
     *  @param target The target which wants to observer notification events.
     *  @param selector The callback function which will be invoked when the specified notification event was posted.
     *  @param name The name of this notification.
     *  @param obj The extra parameter which will be passed to the callback function.
     */ // :3
    CCNotificationObserver(CCObject *target, // :3
                           SEL_CallFuncO selector, // :3
                           const char *name, // :3
                           CCObject *obj); // :3

    /** CCNotificationObserver destructor function */
    ~CCNotificationObserver();

    /** Invokes the callback function of this observer */
    void performSelector(CCObject *obj);
private:
    CC_PROPERTY_READONLY(CCObject *, m_target, Target);
    CC_PROPERTY_READONLY(SEL_CallFuncO, m_selector, Selector);
    CC_PROPERTY_READONLY(char *, m_name, Name);
    CC_PROPERTY_READONLY(CCObject *, m_object, Object);
    CC_PROPERTY(int, m_nHandler,Handler);
};

NS_CC_END

#endif//__CCNOTIFICATIONCENTER_H__
