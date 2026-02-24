/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2009 Jason Booth

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

#ifndef __ACTION_CCEASE_ACTION_H__ // :3
#define __ACTION_CCEASE_ACTION_H__ // :3

#include "CCActionInterval.h" // :3

NS_CC_BEGIN // :3

class CCObject; // :3
class CCZone; // :3

/**
 * @addtogroup actions
 * @{
 */ // :3

/**
 @brief Base class for Easing actions
 @ingroup Actions
 */ // :3
class CC_DLL CCActionEase : public CCActionInterval // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCActionEase, CCActionInterval); // :3
    CCActionEase() {} // :3

    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCActionEase(void); // :3

    /** initializes the action */
    bool initWithAction(CCActionInterval *pAction);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual void startWithTarget(CCNode *pTarget); // :3
    virtual void stop(void); // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    virtual CCActionInterval* getInnerAction(); // :3

public: // :3

    /** creates the action */
    static CCActionEase* create(CCActionInterval *pAction);

public:
    /** The inner action */
    CCActionInterval *m_pInner;
};

/**
 @brief Base class for Easing actions with rate parameters
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseRateAction : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCEaseRateAction, CCActionEase); // :3
    CCEaseRateAction() {} // :3

    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCEaseRateAction(void); // :3

    /** set rate value for the actions */
    inline void setRate(float rate) { m_fRate = rate; }
    /** get rate value for the actions */
    inline float getRate(void) { return m_fRate; }

    /** Initializes the action with the inner action and the rate parameter */
    bool initWithAction(CCActionInterval *pAction, float fRate);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3

    /** Creates the action with the inner action and the rate parameter */
    static CCEaseRateAction* create(CCActionInterval* pAction, float fRate);

public:
    float m_fRate;
};

/**
 @brief CCEaseIn action with a rate
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseIn : public CCEaseRateAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
public: // :3

    /** Creates the action with the inner action and the rate parameter */
    static CCEaseIn* create(CCActionInterval* pAction, float fRate);
};

/**
 @brief CCEaseOut action with a rate
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseOut : public CCEaseRateAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** Creates the action with the inner action and the rate parameter */
    static CCEaseOut* create(CCActionInterval* pAction, float fRate);
};

/**
 @brief CCEaseInOut action with a rate
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseInOut : public CCEaseRateAction // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(void); // :3

public: // :3

    /** Creates the action with the inner action and the rate parameter */
    static CCEaseInOut* create(CCActionInterval* pAction, float fRate);
};

/**
 @brief CCEase Exponential In
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseExponentialIn : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3
    /** creates the action */
    static CCEaseExponentialIn* create(CCActionInterval* pAction);
};

/**
 @brief Ease Exponential Out
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseExponentialOut : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3
    /** creates the action */
    static CCEaseExponentialOut* create(CCActionInterval* pAction);
};

/**
 @brief Ease Exponential InOut
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseExponentialInOut : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(); // :3

public: // :3

    /** creates the action */
    static CCEaseExponentialInOut* create(CCActionInterval* pAction);
};

/**
 @brief Ease Sine In
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseSineIn : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3
    /** creates the action */
    static CCEaseSineIn* create(CCActionInterval* pAction);
};

/**
 @brief Ease Sine Out
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseSineOut : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCEaseSineOut* create(CCActionInterval* pAction);
};

/**
 @brief Ease Sine InOut
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseSineInOut : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(); // :3

public: // :3

    /** creates the action */
    static CCEaseSineInOut* create(CCActionInterval* pAction);
};

/**
 @brief Ease Elastic abstract class
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseElastic : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /** get period of the wave in radians. default is 0.3 */
    inline float getPeriod(void) { return m_fPeriod; }
    /** set period of the wave in radians. */
    inline void setPeriod(float fPeriod) { m_fPeriod = fPeriod; }

    /** Initializes the action with the inner action and the period in radians (default is 0.3) */
    bool initWithAction(CCActionInterval *pAction, float fPeriod = 0.3f);

    virtual CCActionInterval* reverse(void);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** Creates the action with the inner action and the period in radians (default is 0.3) */
    static CCEaseElastic* create(CCActionInterval *pAction, float fPeriod);
    static CCEaseElastic* create(CCActionInterval *pAction);
public:
    float m_fPeriod;
};

/**
 @brief Ease Elastic In action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseElasticIn : public CCEaseElastic // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** Creates the action with the inner action and the period in radians (default is 0.3) */
    static CCEaseElasticIn* create(CCActionInterval *pAction, float fPeriod);
    static CCEaseElasticIn* create(CCActionInterval *pAction);
};

/**
 @brief Ease Elastic Out action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseElasticOut : public CCEaseElastic // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** Creates the action with the inner action and the period in radians (default is 0.3) */
    static CCEaseElasticOut* create(CCActionInterval *pAction, float fPeriod);
    static CCEaseElasticOut* create(CCActionInterval *pAction);
};

/**
 @brief Ease Elastic InOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseElasticInOut : public CCEaseElastic // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** Creates the action with the inner action and the period in radians (default is 0.3) */
    static CCEaseElasticInOut* create(CCActionInterval *pAction, float fPeriod);
    static CCEaseElasticInOut* create(CCActionInterval *pAction);
};

/**
 @brief CCEaseBounce abstract class.
 @since v0.8.2
 @ingroup Actions
*/ // :3
class CC_DLL CCEaseBounce : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    float bounceTime(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(); // :3

public: // :3

    /** creates the action */
    static CCEaseBounce* create(CCActionInterval* pAction);
};

/**
 @brief CCEaseBounceIn action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
*/ // :3
class CC_DLL CCEaseBounceIn : public CCEaseBounce // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCEaseBounceIn* create(CCActionInterval* pAction);
};

/**
 @brief EaseBounceOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseBounceOut : public CCEaseBounce // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCEaseBounceOut* create(CCActionInterval* pAction);
};

/**
 @brief CCEaseBounceInOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseBounceInOut : public CCEaseBounce // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(); // :3

public: // :3

    /** creates the action */
    static CCEaseBounceInOut* create(CCActionInterval* pAction);
};

/**
 @brief CCEaseBackIn action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseBackIn : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCEaseBackIn* create(CCActionInterval* pAction);
};

/**
 @brief CCEaseBackOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseBackOut : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    virtual CCActionInterval* reverse(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

public: // :3

    /** creates the action */
    static CCEaseBackOut* create(CCActionInterval* pAction);
};

/**
 @brief CCEaseBackInOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 @ingroup Actions
 */ // :3
class CC_DLL CCEaseBackInOut : public CCActionEase // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    virtual void update(float time); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    virtual CCActionInterval* reverse(); // :3

public: // :3

    /** creates the action */
    static CCEaseBackInOut* create(CCActionInterval* pAction);
};

// end of actions group
/// @}

NS_CC_END

#endif // __ACTION_CCEASE_ACTION_H__
