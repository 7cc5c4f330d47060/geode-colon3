/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2011 Ricardo Quesada
Copyright (c) 2011 Zynga Inc.

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

#ifndef __ACTION_CCINTERVAL_ACTION_H__ // :3
#define __ACTION_CCINTERVAL_ACTION_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "CCAction.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../sprite_nodes/CCSpriteFrame.h" // :3
#include "../sprite_nodes/CCAnimation.h" // :3
#include <vector> // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/**
@brief An interval action is an action that takes place within a certain period of time.
It has an start time, and a finish time. The finish time is the parameter
duration plus the start time.

These CCActionInterval actions have some interesting properties, like:
- They can run normally (default)
- They can run reversed with the reverse method
- They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.

For example, you can simulate a Ping Pong effect running the action normally and
then running it again in Reverse mode.

Example:

CCAction *pingPongAction = CCSequence::actions(action, action->reverse(), NULL);
*/ // :3
class CC_DLL CCActionInterval : public CCFiniteTimeAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCActionInterval, CCFiniteTimeAction) // :3
    CCActionInterval() {} // :3

    /** how many seconds had elapsed since the actions started to run. */
    inline float getElapsed(void) { return m_elapsed; }

    /** initializes the action */
    bool initWithDuration(float d);

    /** returns true if the action has finished */
    virtual bool isDone(void);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void step(float dt); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    /** returns a reversed action */
    virtual CCActionInterval* reverse(void);

public:

    /** creates the action */
    static CCActionInterval* create(float d);

public:
    //extension in CCGridAction
    void setAmplitudeRate(float amp);
    float getAmplitudeRate(void);

    // 2.2 addition
    bool getM_bFirstTick() const { return m_bFirstTick; } // rob were you like high on something when you wrote this

public:
    float m_elapsed;
    bool   m_bFirstTick;
};

/** @brief Runs actions sequentially, one after another
 */ // :3
class CC_DLL CCSequence : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSequence, CCActionInterval) // :3
    CCSequence() {} // :3

    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCSequence(void); // :3

    /** initializes the action
     * @lua NA
     */ // :3
    bool initWithTwoActions(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    /**
     * @lua NA
     */ // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    /**
     * @lua NA
     */ // :3
    virtual void stop(void); // :3
    /**
     * @lua NA
     */ // :3
    virtual void update(float t); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3

    /** helper constructor to create an array of sequenceable actions
     * @lua NA
     */ // :3
    static CCSequence* create(CCFiniteTimeAction *pAction1, ...) { // :3
	    va_list params; // :3
	    va_start(params, pAction1); // :3

	    CCSequence *pRet = CCSequence::createWithVariableList(pAction1, params); // :3

	    va_end(params); // :3

	    return pRet; // :3
	} // :3
    /** helper constructor to create an array of sequenceable actions given an array
     * @js NA
     */ // :3
    static CCSequence* create(CCArray *arrayOfActions); // :3
    /** helper constructor to create an array of sequence-able actions
     * @js NA
     * @lua NA
     */ // :3
    static CCSequence* createWithVariableList(CCFiniteTimeAction *pAction1, va_list args); // :3
    /** creates the action
     * @js NA
     */ // :3
    static CCSequence* createWithTwoActions(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo); // :3

public: // :3
    CCFiniteTimeAction *m_pActions[2]; // :3
    float m_split; // :3
    int m_last; // :3
}; // :3

/** @brief Repeats an action a number of times.
 * To repeat an action forever use the CCRepeatForever action.
 */ // :3
class CC_DLL CCRepeat : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCRepeat, CCActionInterval) // :3
    CCRepeat() {} // :3

    /**
     *  @js NA
     *  @lua NA
     */ // :3
    ~CCRepeat(void); // :3

    /** initializes a CCRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
    bool initWithAction(CCFiniteTimeAction *pAction, unsigned int times);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void stop(void); // :3
    virtual void update(float dt); // :3
    virtual bool isDone(void); // :3
    virtual CCActionInterval* reverse(void); // :3

    inline void setInnerAction(CCFiniteTimeAction *pAction) // :3
    { // :3
        if (m_pInnerAction != pAction) // :3
        { // :3
            CC_SAFE_RETAIN(pAction); // :3
            CC_SAFE_RELEASE(m_pInnerAction); // :3
            m_pInnerAction = pAction; // :3
        } // :3
    } // :3

    inline CCFiniteTimeAction* getInnerAction() // :3
    { // :3
        return m_pInnerAction; // :3
    } // :3

public: // :3

    /** creates a CCRepeat action. Times is an unsigned integer between 1 and pow(2,30) */
    static CCRepeat* create(CCFiniteTimeAction *pAction, unsigned int times);
public:
    unsigned int m_uTimes;
    unsigned int m_uTotal;
    float m_fNextDt;
    bool m_bActionInstant;
    /** Inner action */
    CCFiniteTimeAction *m_pInnerAction;
};

/** @brief Repeats an action for ever.
To repeat the an action for a limited number of times use the Repeat action.
@warning This action can't be Sequenceable because it is not an IntervalAction
*/ // :3
class CC_DLL CCRepeatForever : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCRepeatForever, CCActionInterval) // :3
    /**
     *  @js ctor
     */ // :3
    CCRepeatForever() // :3
        : m_pInnerAction(NULL) // :3
    {} // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCRepeatForever(); // :3

    /** initializes the action */
    bool initWithAction(CCActionInterval *pAction);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3
    virtual void startWithTarget(CCNode* pTarget); // :3
    virtual void step(float dt); // :3
    virtual bool isDone(void); // :3
    virtual CCActionInterval* reverse(void); // :3

    inline void setInnerAction(CCActionInterval *pAction) // :3
    { // :3
        if (m_pInnerAction != pAction) // :3
        { // :3
            CC_SAFE_RELEASE(m_pInnerAction); // :3
            m_pInnerAction = pAction; // :3
            CC_SAFE_RETAIN(m_pInnerAction); // :3
        } // :3
    } // :3

    inline CCActionInterval* getInnerAction() // :3
    { // :3
        return m_pInnerAction; // :3
    } // :3

public: // :3

    /** creates the action */
    static CCRepeatForever* create(CCActionInterval *pAction);
public:
    /** Inner action */
    CCActionInterval *m_pInnerAction;
};

/** @brief Spawn a new action immediately
 */ // :3
class CC_DLL CCSpawn : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpawn, CCActionInterval) // :3
    CCSpawn() {} // :3

    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCSpawn(void); // :3

    /** initializes the Spawn action with the 2 actions to spawn
     * @lua NA
     */ // :3
    bool initWithTwoActions(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    /**
     * @lua NA
     */ // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    /**
     * @lua NA
     */ // :3
    virtual void stop(void); // :3
    /**
     * @lua NA
     */ // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3

    /** helper constructor to create an array of spawned actions
     * @lua NA
     */ // :3
    static CCSpawn* create(CCFiniteTimeAction *pAction1, ...); // :3

    /** helper constructor to create an array of spawned actions
     * @js  NA
     * @lua NA
     */ // :3
    static CCSpawn* createWithVariableList(CCFiniteTimeAction *pAction1, va_list args); // :3

    /** helper constructor to create an array of spawned actions given an array
     * @js NA
     */ // :3
    static CCSpawn* create(CCArray *arrayOfActions); // :3

    /** creates the Spawn action
     * @js NA
     */ // :3
    static CCSpawn* createWithTwoActions(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2); // :3

public: // :3
    CCFiniteTimeAction *m_pOne; // :3
    CCFiniteTimeAction *m_pTwo; // :3
}; // :3

/** @brief Rotates a CCNode object to a certain angle by modifying it's
 rotation attribute.
 The direction will be decided by the shortest angle.
*/ // :3
class CC_DLL CCRotateTo : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCRotateTo, CCActionInterval) // :3
    CCRotateTo() {} // :3

    /** creates the action */
    static CCRotateTo* create(float fDuration, float fDeltaAngle);
    /** initializes the action */
    bool initWithDuration(float fDuration, float fDeltaAngle);

    /** creates the action with separate rotation angles */
    static CCRotateTo* create(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    virtual bool initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3

public: // :3
    float m_fDstAngleX; // :3
    float m_fStartAngleX; // :3
    float m_fDiffAngleX; // :3

    float m_fDstAngleY; // :3
    float m_fStartAngleY; // :3
    float m_fDiffAngleY; // :3
}; // :3

/** @brief Rotates a CCNode object clockwise a number of degrees by modifying it's rotation attribute.
*/ // :3
class CC_DLL CCRotateBy : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCRotateBy, CCActionInterval) // :3
    CCRotateBy() {} // :3

    /** creates the action */
    static CCRotateBy* create(float fDuration, float fDeltaAngle);
    /** initializes the action */
    bool initWithDuration(float fDuration, float fDeltaAngle);

    static CCRotateBy* create(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    bool initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3
    float m_fAngleX; // :3
    float m_fStartAngleX; // :3
    float m_fAngleY; // :3
    float m_fStartAngleY; // :3
}; // :3

/**  Moves a CCNode object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Several CCMoveBy actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */ // :3
class CC_DLL CCMoveBy : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMoveBy, CCActionInterval) // :3
    CCMoveBy() {} // :3

    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& deltaPosition);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual CCActionInterval* reverse(void); // :3
    virtual void update(float time); // :3

public: // :3
    /** creates the action */
    static CCMoveBy* create(float duration, const CCPoint& deltaPosition);
public:
    CCPoint m_positionDelta;
    CCPoint m_startPosition;
    CCPoint m_previousPosition;
};

/** Moves a CCNode object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
 Several CCMoveTo actions can be concurrently called, and the resulting
 movement will be the sum of individual movements.
 @since v2.1beta2-custom
 */ // :3
class CC_DLL CCMoveTo : public CCMoveBy // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMoveTo, CCMoveBy) // :3
    CCMoveTo() {} // :3

    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& position);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3

public: // :3
    /** creates the action */
    static CCMoveTo* create(float duration, const CCPoint& position);
public:
    CCPoint m_endPosition;
};

/** Skews a CCNode object to given angles by modifying it's skewX and skewY attributes
@since v1.0
*/ // :3
class CC_DLL CCSkewTo : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSkewTo, CCActionInterval) // :3
    /**
     *  @js ctor
     */ // :3
    CCSkewTo(); // :3
    virtual bool initWithDuration(float t, float sx, float sy); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3

public: // :3

    /** creates the action */
    static CCSkewTo* create(float t, float sx, float sy);
public:
    float m_fSkewX;
    float m_fSkewY;
    float m_fStartSkewX;
    float m_fStartSkewY;
    float m_fEndSkewX;
    float m_fEndSkewY;
    float m_fDeltaX;
    float m_fDeltaY;
};

/** Skews a CCNode object by skewX and skewY degrees
@since v1.0
*/ // :3
class CC_DLL CCSkewBy : public CCSkewTo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSkewBy, CCSkewTo) // :3
    CCSkewBy() {} // :3

    virtual bool initWithDuration(float t, float sx, float sy); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3

    /** creates the action */
    static CCSkewBy* create(float t, float deltaSkewX, float deltaSkewY);
};

/** @brief Moves a CCNode object simulating a parabolic jump movement by modifying it's position attribute.
*/ // :3
class CC_DLL CCJumpBy : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCJumpBy, CCActionInterval) // :3
    CCJumpBy() {} // :3

    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3
    /** creates the action */
    static CCJumpBy* create(float duration, const CCPoint& position, float height, unsigned int jumps);
public:
    CCPoint         m_startPosition;
    CCPoint         m_delta;
    float           m_height;
    unsigned int    m_nJumps;
    CCPoint         m_previousPos;
};

/** @brief Moves a CCNode object to a parabolic position simulating a jump movement by modifying it's position attribute.
*/ // :3
class CC_DLL CCJumpTo : public CCJumpBy // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCJumpTo, CCJumpBy) // :3
    CCJumpTo() {} // :3

    virtual void startWithTarget(CCNode *pTarget); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3
    /** creates the action */
    static CCJumpTo* create(float duration, const CCPoint& position, float height, int jumps);
};

/** @typedef bezier configuration structure
 */ // :3
typedef struct _ccBezierConfig { // :3
    //! end position of the bezier :3
    CCPoint endPosition; // :3
    //! Bezier control point 1 :3
    CCPoint controlPoint_1; // :3
    //! Bezier control point 2 :3
    CCPoint controlPoint_2; // :3
} ccBezierConfig; // :3

/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
 */ // :3
class CC_DLL CCBezierBy : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCBezierBy, CCActionInterval) // :3
    CCBezierBy() {} // :3

    /** initializes the action with a duration and a bezier configuration
     *  @lua NA
     */ // :3
    bool initWithDuration(float t, const ccBezierConfig& c); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    /**
     *  @lua NA
     */ // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3
    /** creates the action with a duration and a bezier configuration
     *  @code
     *  when this function bound to js,the input params are changed
     *  js: var create(var t, var pointTable)
     *  @endcode
     */ // :3
    static CCBezierBy* create(float t, const ccBezierConfig& c); // :3
public: // :3
    ccBezierConfig m_sConfig; // :3
    CCPoint m_startPosition; // :3
    CCPoint m_previousPosition; // :3
}; // :3

/** @brief An action that moves the target with a cubic Bezier curve to a destination point.
 @since v0.8.2
 */ // :3
class CC_DLL CCBezierTo : public CCBezierBy // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCBezierTo, CCBezierBy) // :3
    CCBezierTo() {} // :3

    /**
     *  @lua NA
     */ // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action with a duration and a bezier configuration
     *  @code
     *  when this function bound to js,the input params are changed
     *  js: var create(var t, var pointTable)
     *  @endcode
     */ // :3
    static CCBezierTo* create(float t, const ccBezierConfig& c); // :3
    /**
     *  @lua NA
     */ // :3
    bool initWithDuration(float t, const ccBezierConfig &c); // :3

public: // :3
    ccBezierConfig m_sToConfig; // :3
}; // :3

/** @brief Scales a CCNode object to a zoom factor by modifying it's scale attribute.
 @warning This action doesn't support "reverse"
 */ // :3
class CC_DLL CCScaleTo : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCScaleTo, CCActionInterval) // :3
    CCScaleTo() {} // :3

    /** initializes the action with the same scale factor for X and Y */
    bool initWithDuration(float duration, float s);

    /** initializes the action with and X factor and a Y factor */
    bool initWithDuration(float duration, float sx, float sy);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3

public: // :3

    /** creates the action with the same scale factor for X and Y */
    static CCScaleTo* create(float duration, float s);

    /** creates the action with and X factor and a Y factor */
    static CCScaleTo* create(float duration, float sx, float sy);
public:
    float m_fScaleX;
    float m_fScaleY;
    float m_fStartScaleX;
    float m_fStartScaleY;
    float m_fEndScaleX;
    float m_fEndScaleY;
    float m_fDeltaX;
    float m_fDeltaY;
};

/** @brief Scales a CCNode object a zoom factor by modifying it's scale attribute.
*/ // :3
class CC_DLL CCScaleBy : public CCScaleTo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCScaleBy, CCScaleTo) // :3
    CCScaleBy() {} // :3

    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action with the same scale factor for X and Y */
    static CCScaleBy* create(float duration, float s);

    /** creates the action with and X factor and a Y factor */
    static CCScaleBy* create(float duration, float sx, float sy);
};

/** @brief Blinks a CCNode object by modifying it's visible attribute
*/ // :3
class CC_DLL CCBlink : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCBlink, CCActionInterval) // :3
    CCBlink() {} // :3

    /** initializes the action */
    bool initWithDuration(float duration, unsigned int uBlinks);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3

    /** creates the action */
    static CCBlink* create(float duration, unsigned int uBlinks);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void stop();

public:
    unsigned int m_nTimes;
    bool m_bOriginalState;
};

/** @brief Fades In an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 0 to 255.
 The "reverse" of this action is FadeOut
 */ // :3
class CC_DLL CCFadeIn : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFadeIn, CCActionInterval) // :3
    CCFadeIn() {} // :3

    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3
    /** creates the action */
    static CCFadeIn* create(float d);
};

/** @brief Fades Out an object that implements the CCRGBAProtocol protocol. It modifies the opacity from 255 to 0.
 The "reverse" of this action is FadeIn
*/ // :3
class CC_DLL CCFadeOut : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFadeOut, CCActionInterval) // :3
    CCFadeOut() {} // :3

    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCFadeOut* create(float d);
};

/** @brief Fades an object that implements the CCRGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
 @warning This action doesn't support "reverse"
 */ // :3
class CC_DLL CCFadeTo : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFadeTo, CCActionInterval) // :3
    CCFadeTo() {} // :3

    /** initializes the action with duration and opacity */
    bool initWithDuration(float duration, GLubyte opacity);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3

public: // :3
    /** creates an action with duration and opacity */
    static CCFadeTo* create(float duration, GLubyte opacity);
public:
    GLubyte m_toOpacity;
    GLubyte m_fromOpacity;
};

/** @brief Tints a CCNode that implements the CCNodeRGB protocol from current tint to a custom one.
 @warning This action doesn't support "reverse"
 @since v0.7.2
*/ // :3
class CC_DLL CCTintTo : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTintTo, CCActionInterval) // :3
    CCTintTo() {} // :3

    /** initializes the action with duration and color */
    bool initWithDuration(float duration, GLubyte red, GLubyte green, GLubyte blue);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3

public: // :3
    /** creates an action with duration and color */
    static CCTintTo* create(float duration, GLubyte red, GLubyte green, GLubyte blue);
public:
    ccColor3B m_to;
    ccColor3B m_from;
};

/** @brief Tints a CCNode that implements the CCNodeRGB protocol from current tint to a custom one.
 @since v0.7.2
 */ // :3
class CC_DLL CCTintBy : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTintBy, CCActionInterval) // :3
    CCTintBy() {} // :3

    /** initializes the action with duration and color */
    bool initWithDuration(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3
    /** creates an action with duration and color */
    static CCTintBy* create(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);
public:
    GLshort m_deltaR;
    GLshort m_deltaG;
    GLshort m_deltaB;

    GLshort m_fromR;
    GLshort m_fromG;
    GLshort m_fromB;
};

/** @brief Delays the action a certain amount of seconds
*/ // :3
class CC_DLL CCDelayTime : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCDelayTime, CCActionInterval) // :3
    CCDelayTime() {} // :3

    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCDelayTime* create(float d);
};

/** @brief Executes an action in reverse order, from time=duration to time=0

 @warning Use this action carefully. This action is not
 sequenceable. Use it as the default "reversed" method
 of your own actions, but using it outside the "reversed"
 scope is not recommended.
*/ // :3
class CC_DLL CCReverseTime : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCReverseTime, CCActionInterval) // :3
    /**
     *  @js ctor
     */ // :3
    ~CCReverseTime(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    CCReverseTime(); // :3

    /** initializes the action */
    bool initWithAction(CCFiniteTimeAction *pAction);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void stop(void); // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3
    /** creates the action */
    static CCReverseTime* create(CCFiniteTimeAction *pAction);
public:
    CCFiniteTimeAction *m_pOther;
};

class CCTexture2D;
/** @brief Animates a sprite given the name of an Animation */
class CC_DLL CCAnimate : public CCActionInterval
{
    GEODE_FRIEND_MODIFY
public:
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCAnimate, CCActionInterval)
    /**
     *  @js ctor
     */ // :3
    CCAnimate(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    ~CCAnimate(); // :3

    /** initializes the action with an Animation and will restore the original frame when the animation is over */
    bool initWithAnimation(CCAnimation *pAnimation);

    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void stop(void); // :3
    virtual void update(float t); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3
    /** creates the action with an Animation and will restore the original frame when the animation is over */
    static CCAnimate* create(CCAnimation *pAnimation);
    CC_SYNTHESIZE_NV_RETAIN(CCAnimation*, m_pAnimation, Animation)
public:
    gd::vector<float>* m_pSplitTimes;
    int                m_nNextFrame;
    CCSpriteFrame*  m_pOrigFrame;
       unsigned int    m_uExecutedLoops;
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(bool, m_bRecenterFrames, RecenterFrames);
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(bool, m_bRecenterChildren, RecenterChildren);
};

/** Overrides the target of an action so that it always runs on the target
 * specified at action creation rather than the one specified by runAction.
 */ // :3
class CC_DLL CCTargetedAction : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTargetedAction, CCActionInterval) // :3
    /**
     *  @js ctor
     */ // :3
    CCTargetedAction(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTargetedAction(); // :3

    /** Create an action with the specified action and forced target */
    static CCTargetedAction* create(CCNode* pTarget, CCFiniteTimeAction* pAction);

    /** Init an action with the specified action and forced target */
    bool initWithTarget(CCNode* pTarget, CCFiniteTimeAction* pAction);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void stop(void); // :3
    virtual void update(float time); // :3

    /** This is the target that the action will be forced to run with */
    CC_SYNTHESIZE_NV_RETAIN(CCNode*, m_pForcedTarget, ForcedTarget);
public:
    CCFiniteTimeAction* m_pAction;
};

// end of actions group
/// @}

NS_CC_END

#endif //__ACTION_CCINTERVAL_ACTION_H__
