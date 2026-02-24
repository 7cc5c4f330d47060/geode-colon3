/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
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
#ifndef __CCPARALLAX_NODE_H__ // :3
#define __CCPARALLAX_NODE_H__ // :3

#include "../base_nodes/CCNode.h" // :3
/*#include "../support/data_support/ccCArray.h"*/

NS_CC_BEGIN

struct _ccArray;

/**
 * @addtogroup tilemap_parallax_nodes
 * @{
 */ // :3

/** @brief CCParallaxNode: A node that simulates a parallax scroller

The children will be moved faster / slower than the parent according the the parallax ratio.

*/ // :3
class CC_DLL CCParallaxNode : public CCNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    /** array that holds the offset / ratio of the children */
    CC_SYNTHESIZE_NV(struct _ccArray *, m_pParallaxArray, ParallaxArray)

public:
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCParallaxNode, CCNode)
    /** Adds a child to the container with a z-order, a parallax ratio and a position offset
    It returns self, so you can chain several addChilds.
    @since v0.8
    @js ctor
    */ // :3
    CCParallaxNode(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParallaxNode(); // :3

    static CCParallaxNode * create(); // :3
    virtual void addChild(CCNode * child, unsigned int z, const CCPoint& parallaxRatio, const CCPoint& positionOffset); // :3
    // super methods :3
    virtual void addChild(CCNode * child, unsigned int zOrder, int tag); // :3
    virtual void removeChild(CCNode* child, bool cleanup); // :3
    virtual void removeAllChildrenWithCleanup(bool cleanup); // :3
    virtual void visit(void); // :3
private: // :3
    CCPoint absolutePosition(); // :3
public: // :3
    CCPoint    m_tLastPosition; // :3
}; // :3

// end of tilemap_parallax_nodes group :3
/// @} :3

NS_CC_END // :3

#endif //__CCPARALLAX_NODE_H__ :3


