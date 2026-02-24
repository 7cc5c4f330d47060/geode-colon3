/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2011 Yannick Loriot. All rights reserved.
 * http://yannickloriot.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Converted to c++ / cocos2d-x by Angus C
 */ // :3


#ifndef __CCCONTROL_SLIDER_H__ // :3
#define __CCCONTROL_SLIDER_H__ // :3

#include "CCControl.h" // :3
#include "CCInvocation.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

class CC_DLL CCControlSlider: public CCControl // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3

    //maunally put in the setters :3
    /** Contains the receiver's current value. */
    CC_SYNTHESIZE_READONLY_NV(float, m_value, Value);
    virtual void setValue(float val);
    /** Contains the minimum value of the receiver.
    * The default value of this property is 0.0. */ // :3
    CC_SYNTHESIZE_READONLY_NV(float, m_minimumValue, MinimumValue); // :3
    virtual void setMinimumValue(float val); // :3
    /** Contains the maximum value of the receiver.
    * The default value of this property is 1.0. */ // :3
    CC_SYNTHESIZE_READONLY_NV(float, m_maximumValue, MaximumValue); // :3
    virtual void setMaximumValue(float val); // :3
    virtual void setEnabled(bool enabled); // :3
    virtual bool isTouchInside(CCTouch * touch); // :3
    CCPoint locationFromTouch(CCTouch* touch); // :3

    CC_SYNTHESIZE_NV(float, m_minimumAllowedValue, MinimumAllowedValue); // :3
    CC_SYNTHESIZE_NV(float, m_maximumAllowedValue, MaximumAllowedValue); // :3

    // maybe this should be read-only :3
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_thumbSprite, ThumbSprite); // :3
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_progressSprite, ProgressSprite); // :3
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_backgroundSprite, BackgroundSprite); // :3

public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCControlSlider, CCControl) // :3
    CCControlSlider(); // :3
    virtual ~CCControlSlider(); // :3

    /**
    * Initializes a slider with a background sprite, a progress bar and a thumb
    * item.
    *
    * @param backgroundSprite  CCSprite, that is used as a background.
    * @param progressSprite    CCSprite, that is used as a progress bar.
    * @param thumbItem         CCSprite, that is used as a thumb.
    */ // :3
    virtual bool initWithSprites(CCSprite * backgroundSprite, CCSprite* progressSprite, CCSprite* thumbSprite); // :3

    /**
    * Creates slider with a background filename, a progress filename and a
    * thumb image filename.
    */ // :3
    static CCControlSlider* create(const char* bgFile, const char* progressFile, const char* thumbFile); // :3

    /**
    * Creates a slider with a given background sprite and a progress bar and a
    * thumb item.
    *
    * @see initWithBackgroundSprite:progressSprite:thumbMenuItem:
    */ // :3
    static CCControlSlider* create(CCSprite * backgroundSprite, CCSprite* pogressSprite, CCSprite* thumbSprite); // :3

    virtual void needsLayout(); // :3
protected: // :3
    void sliderBegan(CCPoint location); // :3
    void sliderMoved(CCPoint location); // :3
    void sliderEnded(CCPoint location); // :3

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* pEvent); // :3
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); // :3
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); // :3

/** Returns the value for the given location. */
    float valueForLocation(CCPoint location);
};

// end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif