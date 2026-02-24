/*
 * Copyright (c) 2012 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
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
 *
 * Converted to c++ / cocos2d-x by Angus C
 */ // :3

#ifndef __CCCONTROL_UTILS_H__ // :3
#define __CCCONTROL_UTILS_H__ // :3

#include "../../../sprite_nodes/CCSprite.h" // :3
#include "../../ExtensionMacros.h" // :3

NS_CC_EXT_BEGIN // :3

typedef struct // :3
{ // :3
    double r;       // percent :3
    double g;       // percent :3
    double b;       // percent :3
    double a;       // percent :3
} RGBA; // :3

typedef struct // :3
{ // :3
    double h;       // angle in degrees :3
    double s;       // percent :3
    double v;       // percent :3
} HSV; // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */ // :3

/**helper class to store ccColor3B's in mutable arrays
 * @js NA
 * @lua NA
 **/ // :3
class CC_DLL CCColor3bObject : public CCObject // :3
{ // :3
public: // :3
    ccColor3B value; // :3
    CCColor3bObject(ccColor3B s_value):value(s_value){} // :3
}; // :3

class CC_DLL CCControlUtils // :3
{ // :3
public: // :3
    static CCSprite* addSpriteToTargetWithPosAndAnchor(const char* spriteName, CCNode * target, CCPoint pos, CCPoint anchor); // :3
    static HSV HSVfromRGB(RGBA value); // :3
    static RGBA RGBfromHSV(HSV value); // :3
    static CCRect CCRectUnion(const CCRect& src1, const CCRect& src2); // :3
}; // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_EXT_END // :3

#endif // :3