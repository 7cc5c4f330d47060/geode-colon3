/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org

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

#ifndef __CCBOOL_H__ // :3
#define __CCBOOL_H__ // :3

#include "CCObject.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup data_structures
 * @{
 * @js NA
 * @lua NA
 */ // :3

class CC_DLL CCBool : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    CCBool(bool v) // :3
        : m_bValue(v) {} // :3
    bool getValue() const {return m_bValue;} // :3
    bool setValue(bool value) { m_bValue = value; return value; } // :3

    static CCBool* create(bool v) // :3
    { // :3
        CCBool* pRet = new CCBool(v); // :3
        if (pRet) // :3
        { // :3
            pRet->autorelease(); // :3
        } // :3
        return pRet; // :3
    } // :3

    /* override functions */
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

public:
    bool m_bValue;
};

// end of data_structure group
/// @}

NS_CC_END

#endif /* __CCBOOL_H__ */
