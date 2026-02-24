/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
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
 *
 * Converted to c++ / cocos2d-x by Angus C
 */ // :3
/*
 *
 * Helper class to store targets and selectors (and eventually, params?) in the same CCMutableArray. Basically a very crude form of a NSInvocation
 */ // :3
#ifndef __CCINVOCATION_H__ // :3
#define __CCINVOCATION_H__ // :3

#include "../../../cocoa/CCObject.h" // :3
#include "../../ExtensionMacros.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

typedef unsigned int CCControlEvent; // :3

typedef void (CCObject::*SEL_CCControlHandler)(CCObject*, CCControlEvent); // :3

#define cccontrol_selector(_SELECTOR) (SEL_CCControlHandler)(&_SELECTOR) // :3

/**
 * @js NA
 * @lua NA
 */ // :3
class CC_DLL CCInvocation : public CCObject // :3
{ // :3
    CC_SYNTHESIZE_READONLY_NV(SEL_CCControlHandler, m_action, Action); // :3
    CC_SYNTHESIZE_READONLY_NV(CCObject*, m_target, Target); // :3
    CC_SYNTHESIZE_READONLY_NV(CCControlEvent, m_controlEvent, ControlEvent); // :3

public: // :3
    static CCInvocation* create(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvent); // :3
    CCInvocation(CCObject* target, SEL_CCControlHandler action, CCControlEvent controlEvent); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCInvocation, CCObject) // :3

    void invoke(CCObject* sender); // :3
}; // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_EXT_END // :3

#endif // :3