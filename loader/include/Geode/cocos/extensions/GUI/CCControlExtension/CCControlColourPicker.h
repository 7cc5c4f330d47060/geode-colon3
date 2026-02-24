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

#ifndef __CCCONTROL_COLOUR_PICKER_H__ // :3
#define __CCCONTROL_COLOUR_PICKER_H__ // :3


#include "CCControl.h" // :3
#include "CCControlUtils.h" // :3
#include "CCControlHuePicker.h" // :3
#include "CCControlSaturationBrightnessPicker.h" // :3
#include "ColorPickerDelegate.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

class CC_DLL CCControlColourPicker : public CCControl // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3

public: // :3
    // @note RobTop Addition :3
    ccColor3B const& getColorValue() const; // :3
    // @note RobTop Addition :3
    virtual void setColorValue(ccColor3B const&); // :3

    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCControlColourPicker, CCControl) // :3
    CCControlColourPicker(); // :3
    virtual ~CCControlColourPicker(); // :3

public: // :3
    // @note RobTop Addition :3
    ccColor3B m_rgb; // :3
    HSV m_hsv; // :3
    CC_SYNTHESIZE_NV_RETAIN(CCControlSaturationBrightnessPicker*, m_colourPicker, colourPicker) // :3

    CC_SYNTHESIZE_NV_RETAIN(CCControlHuePicker*, m_huePicker, HuePicker) // :3
    CC_SYNTHESIZE_NV_RETAIN(CCSprite*, m_background, Background) // :3

    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(CCSprite*, m_colorTarget, ColorTarget) // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(ColorPickerDelegate*, m_delegate, Delegate) // :3


public: // :3
    // @note RobTop Addition :3
    static CCControlColourPicker* colourPicker(); // :3
    inline static CCControlColourPicker* create() { // :3
        return colourPicker(); // :3
    } // :3

    virtual bool init(); // :3
    void hueSliderValueChanged(CCObject * sender, CCControlEvent controlEvent); // :3
    void colourSliderValueChanged(CCObject * sender, CCControlEvent controlEvent); // :3

protected: // :3
    void updateControlPicker(); // :3
    void updateHueAndControlPicker(); // :3
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* pEvent); // :3

}; // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_EXT_END // :3

#endif // :3
