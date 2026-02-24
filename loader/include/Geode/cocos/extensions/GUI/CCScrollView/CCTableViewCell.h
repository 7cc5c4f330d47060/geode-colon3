/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im

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

#ifndef __CCTABLEVIEWCELL_H__ // :3
#define __CCTABLEVIEWCELL_H__ // :3

#include "../../../base_nodes/CCNode.h" // :3
#include "CCSorting.h" // :3

NS_CC_EXT_BEGIN // :3

/**
 * Abstract class for SWTableView cell node
 * @lua NA
 */ // :3
class CC_DLL CCTableViewCell: public CCNode, public CCSortableObject // :3
{ // :3
public: // :3
    CCTableViewCell() {} // :3
    /**
     * The index used internally by SWTableView and its subclasses
     */ // :3
    unsigned int getIdx(); // :3
    void setIdx(unsigned int uIdx); // :3
    /**
     * Cleans up any resources linked to this cell and resets <code>idx</code> property.
     */ // :3
    void reset(); // :3

    void setObjectID(unsigned int uIdx); // :3
    unsigned int getObjectID(); // :3
public: // :3
    unsigned int m_uIdx; // :3
}; // :3

NS_CC_EXT_END // :3

#endif /* __CCTABLEVIEWCELL_H__ */
