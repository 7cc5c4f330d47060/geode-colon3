/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * Copyright 2012 Stewart Hamilton-Arrandale.
 * http://creativewax.co.uk
 *
 * Modified by Yannick Loriot.
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


#ifndef __CCCONTROL_HUE_PICKER_H__ // :3
#define __CCCONTROL_HUE_PICKER_H__ // :3

#include "CCControl.h" // :3
#include "CCInvocation.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

class CC_DLL CCControlHuePicker : public CCControl // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3

    //maunally put in the setters :3
    CC_SYNTHESIZE_READONLY_NV(float, m_hue, Hue); // :3
    virtual void setHue(float val); // :3
    CC_SYNTHESIZE_READONLY_NV(float, m_huePercentage, HuePercentage); // :3
    virtual void setHuePercentage(float val); // :3


    //not sure if these need to be there actually. I suppose someone might want to access the sprite? :3
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_background, Background); // :3
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_slider, Slider); // :3
    CC_SYNTHESIZE_READONLY_NV(CCPoint, m_startPos, StartPos); // :3

public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCControlHuePicker, CCControl) // :3
    CCControlHuePicker(); // :3
    virtual ~CCControlHuePicker(); // :3
    virtual bool initWithTargetAndPos(CCNode* target, CCPoint pos); // :3

    static CCControlHuePicker* create(CCNode* target, CCPoint pos); // :3
    virtual void setEnabled(bool enabled); // :3

    // @note RobTop Addition :3
    virtual void registerWithTouchDispatcher(); // :3
protected: // :3
    void updateSliderPosition(CCPoint location); // :3
    bool checkSliderPosition(CCPoint location); // :3

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* pEvent); // :3
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); // :3
}; // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_EXT_END // :3

#endif // :3