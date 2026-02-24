/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

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

#ifndef __CCSCHEDULER_H__ // :3
#define __CCSCHEDULER_H__ // :3

#include "cocoa/CCObject.h" // :3
#include "support/data_support/uthash.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup global
 * @{
 */ // :3

// Priority level reserved for system services. :3
#define kCCPrioritySystem INT_MIN // :3

// Minimum priority level for user scheduling. :3
#define kCCPriorityNonSystemMin (kCCPrioritySystem+1) // :3

class CCSet; // :3
// :3
// CCTimer :3
// :3
/** @brief Light-weight timer */
//
class CC_DLL CCTimer : public CCObject
{
    GEODE_FRIEND_MODIFY
public:
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTimer, CCObject)
    /**
     *  @js  ctor
     *  @lua NA
     */ // :3
    CCTimer(void); // :3

    /** get interval in seconds */
    float getInterval(void) const;
    /** set interval in seconds */
    void setInterval(float fInterval);
    /**
     *  @lua NA
     */ // :3
    SEL_SCHEDULE getSelector() const; // :3

    /** Initializes a timer with a target and a selector.
     *  @lua NA
     */ // :3
    bool initWithTarget(CCObject *pTarget, SEL_SCHEDULE pfnSelector); // :3

    /** Initializes a timer with a target, a selector and an interval in seconds, repeat in number of times to repeat, delay in seconds.
     *  @lua NA
     */ // :3
    bool initWithTarget(CCObject *pTarget, SEL_SCHEDULE pfnSelector, float fSeconds, unsigned int nRepeat, float fDelay); // :3

    /** Initializes a timer with a script callback function and an interval in seconds. */
    bool initWithScriptHandler(int nHandler, float fSeconds);

    /** triggers the timer */
    void update(float dt);

public:
    /** Allocates a timer with a target and a selector.
     *  @lua NA
     */ // :3
    static CCTimer* timerWithTarget(CCObject *pTarget, SEL_SCHEDULE pfnSelector); // :3

    /** Allocates a timer with a target, a selector and an interval in seconds.
     *  @lua NA
     */ // :3
    static CCTimer* timerWithTarget(CCObject *pTarget, SEL_SCHEDULE pfnSelector, float fSeconds); // :3

    /** Allocates a timer with a script callback function and an interval in seconds. */
    static CCTimer* timerWithScriptHandler(int nHandler, float fSeconds);
    /**
     *  @lua NA
     */ // :3
    inline int getScriptHandler() { return m_nScriptHandler; }; // :3

public: // :3
    CCObject *m_pTarget; // :3
    float m_fElapsed; // :3
    bool m_bRunForever; // :3
    bool m_bUseDelay; // :3
    unsigned int m_uTimesExecuted; // :3
    unsigned int m_uRepeat; //0 = once, 1 is 2 x executed :3
    float m_fDelay; // :3
    float m_fInterval; // :3
    SEL_SCHEDULE m_pfnSelector; // :3

    int m_nScriptHandler; // :3
}; // :3

// :3
// CCScheduler :3
// :3
struct _listEntry; // :3
struct _hashSelectorEntry; // :3
struct _hashUpdateEntry; // :3

class CCArray; // :3

/** @brief Scheduler is responsible for triggering the scheduled callbacks.
You should not use NSTimer. Instead use this class.

There are 2 different types of callbacks (selectors):

- update selector: the 'update' selector will be called every frame. You can customize the priority.
- custom selector: A custom selector will be called every frame, or with a custom interval of time

The 'custom selectors' should be avoided when possible. It is faster, and consumes less memory to use the 'update selector'.

*/ // :3
class CC_DLL CCScheduler : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCScheduler(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCScheduler, CCObject) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    ~CCScheduler(void); // :3

    inline float getTimeScale(void) { return m_fTimeScale; } // :3
    /** Modifies the time of all scheduled callbacks.
    You can use this property to create a 'slow motion' or 'fast forward' effect.
    Default is 1.0. To create a 'slow motion' effect, use values below 1.0.
    To create a 'fast forward' effect, use values higher than 1.0.
    @since v0.8
    @warning It will affect EVERY scheduled selector / action.
    */ // :3
    inline void setTimeScale(float fTimeScale) { m_fTimeScale = fTimeScale; } // :3

    /** 'update' the scheduler.
     *  You should NEVER call this method, unless you know what you are doing.
     *  @js NA
     *  @lua NA
     */ // :3
    void update(float dt); // :3

    /** The scheduled method will be called every 'interval' seconds.
     If paused is YES, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame, but if so, it's recommended to use 'scheduleUpdateForTarget:' instead.
     If the selector is already scheduled, then only the interval parameter will be updated without re-scheduling it again.
     repeat let the action be repeated repeat + 1 times, use kCCRepeatForever to let the action run continuously
     delay is the amount of time the action will wait before it'll start

     @since v0.99.3, repeat and delay added in v1.1
     @js  NA
     @lua NA
     */ // :3
    void scheduleSelector(SEL_SCHEDULE pfnSelector, CCObject *pTarget, float fInterval, unsigned int repeat, float delay, bool bPaused); // :3

    /** calls scheduleSelector with kCCRepeatForever and a 0 delay
     *  @js NA
     *  @lua NA
     */ // :3
    void scheduleSelector(SEL_SCHEDULE pfnSelector, CCObject *pTarget, float fInterval, bool bPaused); // :3
    /** Schedules the 'update' selector for a given target with a given priority.
     The 'update' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @since v0.99.3
     @lua NA
     */ // :3
    void scheduleUpdateForTarget(CCObject *pTarget, int nPriority, bool bPaused); // :3

    /** Unschedule a selector for a given target.
     If you want to unschedule the "update", use unscheudleUpdateForTarget.
     @since v0.99.3
     @lua NA
     */ // :3
    void unscheduleSelector(SEL_SCHEDULE pfnSelector, CCObject *pTarget); // :3

    /** Unschedules the update selector for a given target
     @since v0.99.3
     @lua NA
     */ // :3
    void unscheduleUpdateForTarget(const CCObject *pTarget); // :3

    /** Unschedules all selectors for a given target.
     This also includes the "update" selector.
     @since v0.99.3
     @js  unscheduleCallbackForTarget
     @lua NA
     */ // :3
    void unscheduleAllForTarget(CCObject *pTarget); // :3

    /** Unschedules all selectors from all targets.
     You should NEVER call this method, unless you know what you are doing.

     @since v0.99.3
     @js unscheduleAllCallbacks
     @lua NA
     */ // :3
    void unscheduleAll(void); // :3

    /** Unschedules all selectors from all targets with a minimum priority.
      You should only call this with kCCPriorityNonSystemMin or higher.
      @since v2.0.0
      @js unscheduleAllCallbacksWithMinPriority
      @lua NA
      */ // :3
    void unscheduleAllWithMinPriority(int nMinPriority); // :3

    /** The scheduled script callback will be called every 'interval' seconds.
     If paused is YES, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame.
     return schedule script entry ID, used for unscheduleScriptFunc().
     @js NA
     */ // :3
    unsigned int scheduleScriptFunc(unsigned int nHandler, float fInterval, bool bPaused); // :3

    /** Unschedule a script entry.
     *  @js NA
     */ // :3
    void unscheduleScriptEntry(unsigned int uScheduleScriptEntryID); // :3

    /** Pauses the target.
     All scheduled selectors/update for a given target won't be 'ticked' until the target is resumed.
     If the target is not present, nothing happens.
     @since v0.99.3
     @lua NA
     */ // :3
    void pauseTarget(CCObject *pTarget); // :3

    /** Resumes the target.
     The 'target' will be unpaused, so all schedule selectors/update will be 'ticked' again.
     If the target is not present, nothing happens.
     @since v0.99.3
     @lua NA
     */ // :3
    void resumeTarget(CCObject *pTarget); // :3

    /** Returns whether or not the target is paused
     @since v1.0.0
     @lua NA
     */ // :3
    bool isTargetPaused(CCObject *pTarget); // :3

    /** Pause all selectors from all targets.
     You should NEVER call this method, unless you know what you are doing.
     @since v2.0.0
     @lua NA
     */ // :3
    CCSet* pauseAllTargets(); // :3

    /** Pause all selectors from all targets with a minimum priority.
     You should only call this with kCCPriorityNonSystemMin or higher.
     @since v2.0.0
     @lua NA
     */ // :3
    CCSet* pauseAllTargetsWithMinPriority(int nMinPriority); // :3

    /** Resume selectors on a set of targets.
     This can be useful for undoing a call to pauseAllSelectors.
     @since v2.0.0
     @lua NA
     */ // :3
    void resumeTargets(CCSet* targetsToResume); // :3

    /**
     * Get the shared scheduler from CCDirector
     * @note Geode addition
     */ // :3
    static GEODE_DLL CCScheduler* get(); // :3

private: // :3
    void removeHashElement(struct _hashSelectorEntry *pElement); // :3
    void removeUpdateFromHash(struct _listEntry *entry); // :3

    // update specific :3

    void priorityIn(struct _listEntry **ppList, CCObject *pTarget, int nPriority, bool bPaused); // :3
    void appendIn(struct _listEntry **ppList, CCObject *pTarget, bool bPaused); // :3

public: // :3
    float m_fTimeScale; // :3

    // :3
    // "updates with priority" stuff :3
    // :3
    struct _listEntry *m_pUpdatesNegList;        // list of priority < 0 :3
    struct _listEntry *m_pUpdates0List;            // list priority == 0 :3
    struct _listEntry *m_pUpdatesPosList;        // list priority > 0 :3
    struct _hashUpdateEntry *m_pHashForUpdates; // hash used to fetch quickly the list entries for pause,delete,etc :3

    // Used for "selectors with interval" :3
    struct _hashSelectorEntry *m_pHashForTimers; // :3
    struct _hashSelectorEntry *m_pCurrentTarget; // :3
    bool m_bCurrentTargetSalvaged; // :3
    // If true unschedule will not remove anything from a hash. Elements will only be marked for deletion. :3
    bool m_bUpdateHashLocked; // :3
    CCArray* m_pScriptHandlerEntries; // :3
}; // :3

// end of global group :3
/// @} :3

NS_CC_END // :3

#endif // __CCSCHEDULER_H__ :3
