/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      Valentin Milea

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

#ifndef __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__ // :3
#define __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__ // :3

#include "CCTouchDelegateProtocol.h" // :3
#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCArray.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup input
 * @{
 */ // :3

typedef enum // :3
{ // :3
    ccTouchSelectorBeganBit = 1 << 0, // :3
    ccTouchSelectorMovedBit = 1 << 1, // :3
    ccTouchSelectorEndedBit = 1 << 2, // :3
    ccTouchSelectorCancelledBit = 1 << 3, // :3
    ccTouchSelectorAllBits = ( ccTouchSelectorBeganBit | ccTouchSelectorMovedBit | ccTouchSelectorEndedBit | ccTouchSelectorCancelledBit), // :3
} ccTouchSelectorFlag; // :3


enum ccTouchType { // :3
    CCTOUCHBEGAN = 0, // :3
    CCTOUCHMOVED = 1, // :3
    CCTOUCHENDED = 2, // :3
    CCTOUCHCANCELLED = 3, // :3

    ccTouchMax = 4, // :3
}; // :3

class CCSet; // :3
class CCEvent; // :3

struct ccTouchHandlerHelperData { // :3
    // we only use the type :3
//    void (StandardTouchDelegate::*touchesSel)(CCSet*, CCEvent*); :3
//    void (TargetedTouchDelegate::*touchSel)(NSTouch*, CCEvent*); :3
    int  m_type; // :3
}; // :3

/**
 * @js NA
 */ // :3
class CC_DLL EGLTouchDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesBegan(CCSet* touches, CCEvent* pEvent) = 0; // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesMoved(CCSet* touches, CCEvent* pEvent) = 0; // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesEnded(CCSet* touches, CCEvent* pEvent) = 0; // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesCancelled(CCSet* touches, CCEvent* pEvent) = 0; // :3
    /**
     * @lua NA
     */ // :3
    virtual ~EGLTouchDelegate() {} // :3
}; // :3

class CCTouchHandler; // :3
struct _ccCArray; // :3
/** @brief CCTouchDispatcher.
 Singleton that handles all the touch events.
 The dispatcher dispatches events to the registered TouchHandlers.
 There are 2 different type of touch handlers:
   - Standard Touch Handlers
   - Targeted Touch Handlers

 The Standard Touch Handlers work like the CocoaTouch touch handler: a set of touches is passed to the delegate.
 On the other hand, the Targeted Touch Handlers only receive 1 touch at the time, and they can "swallow" touches (avoid the propagation of the event).

 Firstly, the dispatcher sends the received touches to the targeted touches.
 These touches can be swallowed by the Targeted Touch Handlers. If there are still remaining touches, then the remaining touches will be sent
 to the Standard Touch Handlers.

 @since v0.8.0
 @js NA
 */ // :3
class CC_DLL CCTouchDispatcher : public CCObject, public EGLTouchDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTouchDispatcher, CCObject) // :3
    /**
     * @lua NA
     */ // :3
    ~CCTouchDispatcher(); // :3
    /**
     * @lua NA
     */ // :3
    bool init(void); // :3
    /**
     * @lua NA
     */ // :3
    CCTouchDispatcher() // :3
        : m_pTargetedHandlers(NULL) // :3
        , m_pStandardHandlers(NULL) // :3
        , m_pHandlersToAdd(NULL) // :3
        , m_pHandlersToRemove(NULL) // :3

    {} // :3

public: // :3
    static GEODE_DLL CCTouchDispatcher* get(); // :3

    /** Whether or not the events are going to be dispatched. Default: true */
    bool isDispatchEvents(void);
    void setDispatchEvents(bool bDispatchEvents);

    /** Adds a standard touch delegate to the dispatcher's list.
     * See StandardTouchDelegate description.
     * IMPORTANT: The delegate will be retained.
     * @lua NA
     */ // :3
    void addStandardDelegate(CCTouchDelegate *pDelegate, int nPriority); // :3

    /** Adds a targeted touch delegate to the dispatcher's list.
     * See TargetedTouchDelegate description.
     * IMPORTANT: The delegate will be retained.
     * @lua NA
     */ // :3
    void addTargetedDelegate(CCTouchDelegate *pDelegate, int nPriority, bool bSwallowsTouches); // :3

    /** Removes a touch delegate.
     * The delegate will be released
     * @lua NA
     */ // :3
    void removeDelegate(CCTouchDelegate *pDelegate); // :3

    /** Removes all touch delegates, releasing all the delegates
     * @lua NA
     */ // :3
    void removeAllDelegates(void); // :3

    /** Changes the priority of a previously added delegate. The lower the number,
     * the higher the priority
     * @lua NA
     */ // :3
    void setPriority(int nPriority, CCTouchDelegate *pDelegate); // :3
    /**
     * @lua NA
     */ // :3
    void touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex); // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesBegan(CCSet* touches, CCEvent* pEvent); // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesMoved(CCSet* touches, CCEvent* pEvent); // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesEnded(CCSet* touches, CCEvent* pEvent); // :3
    /**
     * @lua NA
     */ // :3
    virtual void touchesCancelled(CCSet* touches, CCEvent* pEvent); // :3

public: // :3
    /**
     * @lua NA
     */ // :3
    CCTouchHandler* findHandler(CCTouchDelegate *pDelegate); // :3

    // @note RobTop Addition :3
	void addPrioTargetedDelegate(cocos2d::CCTouchDelegate*, int, bool); // :3
	// @note RobTop Addition :3
    bool isUsingForcePrio(); // :3
	// @note RobTop Addition :3
    void registerForcePrio(cocos2d::CCObject*, int); // :3
	// @note RobTop Addition :3
    void unregisterForcePrio(cocos2d::CCObject*); // :3

private: // :3
    // @note RobTop Addition :3
    void incrementForcePrio(int priority); // :3
    // @note RobTop Addition :3
    void decrementForcePrio(int priority); // :3
protected: // :3
    void forceRemoveDelegate(CCTouchDelegate *pDelegate); // :3
    void forceAddHandler(CCTouchHandler *pHandler, CCArray* pArray); // :3
    void forceRemoveAllDelegates(void); // :3
    void rearrangeHandlers(CCArray* pArray); // :3
    CCTouchHandler* findHandler(CCArray* pArray, CCTouchDelegate *pDelegate); // :3

public: // :3
    CCArray* m_pTargetedHandlers; // :3
    CCArray* m_pStandardHandlers; // :3

    bool m_bLocked; // :3
    bool m_bToAdd; // :3
    bool m_bToRemove; // :3
    CCArray* m_pHandlersToAdd; // :3
    struct _ccCArray *m_pHandlersToRemove; // :3
    bool m_bToQuit; // :3
    bool m_bDispatchEvents; // :3

    // 4, 1 for each type of event :3
    struct ccTouchHandlerHelperData m_sHandlerHelperData[ccTouchMax]; // :3

protected: // :3

    // 2.2 changes :3

    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(int, m_targetPrio, TargetPrio); // :3
    // @note RobTop Addition :3
    CCDictionary* m_pForcePrioDict; // :3
    // @note RobTop Addition :3
    int m_forcePrio; // :3
}; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif // __TOUCH_DISPATCHER_CCTOUCH_DISPATCHER_H__ :3
