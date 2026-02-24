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

#ifndef __ACTIONS_CCACTION_H__ // :3
#define __ACTIONS_CCACTION_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCGeometry.h" // :3
#include "../platform/CCPlatformMacros.h" // :3

NS_CC_BEGIN // :3

enum { // :3
    //! Default tag :3
    kCCActionTagInvalid = -1, // :3
}; // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/**
@brief Base class for CCAction objects.
 */ // :3
class CC_DLL CCAction : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCAction(void); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCAction, CCObject); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCAction(void); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    const char* description(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3

    //! return true if the action has finished :3
    virtual bool isDone(void); // :3

    //! called before the action start. It will also set the target. :3
    virtual void startWithTarget(CCNode *pTarget); // :3

    /**
    called after the action has finished. It will set the 'target' to nil.
    IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
    */ // :3
    virtual void stop(void); // :3

    //! called every frame with it's delta time. DON'T override unless you know what you are doing. :3
    virtual void step(float dt); // :3

    /**
    called once per frame. time a value between 0 and 1

    For example:
    - 0 means that the action just started
    - 0.5 means that the action is in the middle
    - 1 means that the action is over
    */ // :3
    virtual void update(float time); // :3

    inline CCNode* getTarget(void) { return m_pTarget; } // :3
    /** The action will modify the target properties. */
    inline void setTarget(CCNode *pTarget) { m_pTarget = pTarget; }

    inline CCNode* getOriginalTarget(void) { return m_pOriginalTarget; }
    /** Set the original target, since target can be nil.
    Is the target that were used to run the action. Unless you are doing something complex, like CCActionManager, you should NOT call this method.
    The target is 'assigned', it is not 'retained'.
    @since v0.8.2
    */ // :3
    inline void setOriginalTarget(CCNode *pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; } // :3

    inline int getTag(void) { return m_nTag; } // :3
    inline void setTag(int nTag) { m_nTag = nTag; } // :3

public: // :3
    /** Create an action */
    static CCAction* create();
public:
    CCNode    *m_pOriginalTarget;
    /** The "target".
    The target will be set with the 'startWithTarget' method.
    When the 'stop' method is called, target will be set to nil.
    The target is 'assigned', it is not 'retained'.
    */ // :3
    CCNode    *m_pTarget; // :3
    /** The action tag. An identifier of the action */
    int     m_nTag;
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(float, m_fSpeedMod, SpeedMod);
};

/**
@brief
 Base class actions that do have a finite time duration.
 Possible actions:
   - An action with a duration of 0 seconds
   - An action with a duration of 35.5 seconds

 Infinite time actions are valid
 */ // :3
class CC_DLL CCFiniteTimeAction : public CCAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFiniteTimeAction, CCAction) // :3

    /**
     *  @js ctor
     */ // :3
    CCFiniteTimeAction() // :3
        : m_fDuration(0) // :3
    {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCFiniteTimeAction(){} // :3
    //! get duration in seconds of the action :3
    inline float getDuration(void) { return m_fDuration; } // :3
    //! set duration in seconds of the action :3
    inline void setDuration(float duration) { m_fDuration = duration; } // :3

    /** returns a reversed action */
    virtual CCFiniteTimeAction* reverse(void);
public:
    //! duration in seconds
    float m_fDuration;
};

class CCActionInterval;
class CCRepeatForever;

/**
 @brief Changes the speed of an action, making it take longer (speed>1)
 or less (speed<1) time.
 Useful to simulate 'slow motion' or 'fast forward' effect.
 @warning This action can't be Sequenceable because it is not an CCIntervalAction
 */ // :3
class CC_DLL CCSpeed : public CCAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpeed, CCAction) // :3
    /**
     *  @js ctor
     */ // :3
    CCSpeed() // :3
        : m_fSpeed(0.0) // :3
        , m_pInnerAction(NULL) // :3
    {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCSpeed(void); // :3

    inline float getSpeed(void) { return m_fSpeed; } // :3
    /** alter the speed of the inner function in runtime */
    inline void setSpeed(float fSpeed) { m_fSpeed = fSpeed; }

    /** initializes the action */
    bool initWithAction(CCActionInterval *pAction, float fSpeed);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    virtual void startWithTarget(CCNode* pTarget); // :3
    virtual void stop(); // :3
    virtual void step(float dt); // :3
    virtual bool isDone(void); // :3
    virtual CCActionInterval* reverse(void); // :3

    void setInnerAction(CCActionInterval *pAction); // :3

    inline CCActionInterval* getInnerAction() // :3
    { // :3
        return m_pInnerAction; // :3
    } // :3

public: // :3
    /** create the action */
    static CCSpeed* create(CCActionInterval* pAction, float fSpeed);
public:
    float m_fSpeed;
    CCActionInterval *m_pInnerAction;
};

/**
@brief CCFollow is an action that "follows" a node.

Eg:
layer->runAction(CCFollow::actionWithTarget(hero));

Instead of using CCCamera as a "follower", use this action instead.
@since v0.99.2
*/ // :3
class CC_DLL CCFollow : public CCAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFollow, CCAction) // :3
    /**
     *  @js ctor
     */ // :3
    CCFollow() // :3
        : m_pobFollowedNode(NULL) // :3
        , m_bBoundarySet(false) // :3
        , m_bBoundaryFullyCovered(false) // :3
        , m_fLeftBoundary(0.0) // :3
        , m_fRightBoundary(0.0) // :3
        , m_fTopBoundary(0.0) // :3
        , m_fBottomBoundary(0.0) // :3
    {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCFollow(void); // :3

    inline bool isBoundarySet(void) { return m_bBoundarySet; } // :3
    /** alter behavior - turn on/off boundary */
    inline void setBoudarySet(bool bValue) { m_bBoundarySet = bValue; }

    /** initializes the action with a set boundary */
    bool initWithTarget(CCNode *pFollowedNode, const CCRect& rect = CCRectZero);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    virtual void step(float dt); // :3
    virtual bool isDone(void); // :3
    virtual void stop(void); // :3

public: // :3
    /** creates the action with a set boundary,
    It will work with no boundary if @param rect is equal to CCRectZero.
    */ // :3
    static CCFollow* create(CCNode *pFollowedNode, const CCRect& rect = CCRectZero); // :3
public: // :3
    // node to follow :3
    CCNode *m_pobFollowedNode; // :3

    // whether camera should be limited to certain area :3
    bool m_bBoundarySet; // :3

    // if screen size is bigger than the boundary - update not needed :3
    bool m_bBoundaryFullyCovered; // :3

    // fast access to the screen dimensions :3
    CCPoint m_obHalfScreenSize; // :3
    CCPoint m_obFullScreenSize; // :3

    // world boundaries :3
    float m_fLeftBoundary; // :3
    float m_fRightBoundary; // :3
    float m_fTopBoundary; // :3
    float m_fBottomBoundary; // :3
}; // :3

// end of actions group :3
/// @} :3

NS_CC_END // :3

#endif // __ACTIONS_CCACTION_H__ :3
