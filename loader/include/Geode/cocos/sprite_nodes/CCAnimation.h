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
#ifndef __CC_ANIMATION_H__ // :3
#define __CC_ANIMATION_H__ // :3

#include "../platform/CCPlatformConfig.h" // :3
#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCArray.h" // :3
#include "../cocoa/CCDictionary.h" // :3
#include "../cocoa/CCGeometry.h" // :3
#include "CCSpriteFrame.h" // :3
#include <string> // :3

NS_CC_BEGIN // :3

class CCTexture2D; // :3
class CCSpriteFrame; // :3

/**
 * @addtogroup sprite_nodes
 * @{
 */ // :3

/** CCAnimationFrame
 A frame of the animation. It contains information like:
    - sprite frame name
    - # of delay units.
    - offset

 @since v2.0
 */ // :3
class CC_DLL CCAnimationFrame : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCAnimationFrame, CCObject) // :3
    /**
     * @js ctor
     */ // :3
    CCAnimationFrame(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCAnimationFrame(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3
    /** initializes the animation frame with a spriteframe, number of delay units and a notification user info */
    bool initWithSpriteFrame(CCSpriteFrame* spriteFrame, float delayUnits, CCDictionary* userInfo);

    /** CCSpriteFrameName to be used */
    CC_SYNTHESIZE_NV_RETAIN(CCSpriteFrame*, m_pSpriteFrame, SpriteFrame)

    /**  how many units of time the frame takes */
    CC_SYNTHESIZE_NV(float, m_fDelayUnits, DelayUnits)

    /**  A CCAnimationFrameDisplayedNotification notification will be broadcast when the frame is displayed with this dictionary as UserInfo. If UserInfo is nil, then no notification will be broadcast. */
    CC_SYNTHESIZE_NV_RETAIN(CCDictionary*, m_pUserInfo, UserInfo)
};




/** A CCAnimation object is used to perform animations on the CCSprite objects.

The CCAnimation object contains CCAnimationFrame objects, and a possible delay between the frames.
You can animate a CCAnimation object by using the CCAnimate action. Example:

[sprite runAction:[CCAnimate actionWithAnimation:animation]];

*/ // :3
class CC_DLL CCAnimation : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCAnimation, CCObject) // :3
    /**
     * @js ctor
     */ // :3
    CCAnimation(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCAnimation(void); // :3
public: // :3
    /** Creates an animation
    @since v0.99.5
    */ // :3
    static CCAnimation* create(void); // :3

    /* Creates an animation with an array of CCSpriteFrame and a delay between frames in seconds.
     The frames will be added with one "delay unit".
     @since v0.99.5
     @js create
    */ // :3
    static CCAnimation* createWithSpriteFrames(CCArray* arrayOfSpriteFrameNames, float delay = 0.0f); // :3

    /* Creates an animation with an array of CCAnimationFrame, the delay per units in seconds and and how many times it should be executed.
     @since v2.0
     */ // :3
    static CCAnimation* create(CCArray *arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops); // :3
    static CCAnimation* create(CCArray *arrayOfAnimationFrameNames, float delayPerUnit) { // :3
        return CCAnimation::create(arrayOfAnimationFrameNames, delayPerUnit, 1); // :3
    } // :3

    /** Adds a CCSpriteFrame to a CCAnimation.
     The frame will be added with one "delay unit".
    */ // :3
    void addSpriteFrame(CCSpriteFrame *pFrame); // :3

    /** Adds a frame with an image filename. Internally it will create a CCSpriteFrame and it will add it.
     The frame will be added with one "delay unit".
     Added to facilitate the migration from v0.8 to v0.9.
     * @js addSpriteFrameWithFile
     */ // :3
    void addSpriteFrameWithFileName(const char *pszFileName); // :3

    /** Adds a frame with a texture and a rect. Internally it will create a CCSpriteFrame and it will add it.
     The frame will be added with one "delay unit".
     Added to facilitate the migration from v0.8 to v0.9.
     */ // :3
    void addSpriteFrameWithTexture(CCTexture2D* pobTexture, const CCRect& rect); // :3
    /**
     * @lua NA
     */ // :3
    bool init(); // :3

    /** Initializes a CCAnimation with frames and a delay between frames
     @since v0.99.5
     @lua NA
    */ // :3
    bool initWithSpriteFrames(CCArray *pFrames, float delay = 0.0f); // :3

    /** Initializes a CCAnimation with CCAnimationFrame
     @since v2.0
     @lua NA
    */ // :3
    bool initWithAnimationFrames(CCArray* arrayOfAnimationFrames, float delayPerUnit, unsigned int loops); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone* pZone); // :3

    /** total Delay units of the CCAnimation. */
    CC_SYNTHESIZE_READONLY_NV(float, m_fTotalDelayUnits, TotalDelayUnits)

    /** Delay in seconds of the "delay unit" */
    CC_SYNTHESIZE_NV(float, m_fDelayPerUnit, DelayPerUnit)

    /** duration in seconds of the whole animation. It is the result of totalDelayUnits * delayPerUnit */
    CC_PROPERTY_READONLY(float, m_fDuration, Duration)

    /** array of CCAnimationFrames */
    CC_SYNTHESIZE_NV_RETAIN(CCArray*, m_pFrames, Frames)

    /** whether or not it shall restore the original frame when the animation finishes */
    CC_SYNTHESIZE_NV(bool, m_bRestoreOriginalFrame, RestoreOriginalFrame)

    /** how many times the animation is going to loop. 0 means animation is not animated. 1, animation is executed one time, ... */
    CC_SYNTHESIZE_NV(unsigned int, m_uLoops, Loops)
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_ANIMATION_H__
