/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 * cocos2d-x: http://www.cocos2d-x.org
 *
 * Copyright (c) 2012 Pierre-David Bélanger
 * Copyright (c) 2012 cocos2d-x.org
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
 */ // :3

#ifndef __MISCNODE_CCCLIPPING_NODE_H__ // :3
#define __MISCNODE_CCCLIPPING_NODE_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../platform/CCGL.h" // :3

NS_CC_BEGIN // :3

/** CCClippingNode is a subclass of CCNode.
 It draws its content (childs) clipped using a stencil.
 The stencil is an other CCNode that will not be drawn.
 The clipping is done using the alpha part of the stencil (adjusted with an alphaThreshold).
 */ // :3
class CC_DLL CCClippingNode : public CCNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3


    CCNode* m_pStencil; // :3
    GLfloat m_fAlphaThreshold; // :3
    bool    m_bInverted; // :3

public: // :3
    /** Creates and initializes a clipping node without a stencil.
     */ // :3
    static CCClippingNode* create(); // :3

    /** Creates and initializes a clipping node with an other node as its stencil.
     The stencil node will be retained.
     */ // :3
    static CCClippingNode* create(CCNode *pStencil); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCClippingNode(); // :3

    /** Initializes a clipping node without a stencil.
     */ // :3
    virtual bool init(); // :3

    /** Initializes a clipping node with an other node as its stencil.
     The stencil node will be retained, and its parent will be set to this clipping node.
     */ // :3
    virtual bool init(CCNode *pStencil); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnterTransitionDidFinish(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onExitTransitionDidStart(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onExit(); // :3
    virtual void visit(); // :3

    /** The CCNode to use as a stencil to do the clipping.
     The stencil node will be retained.
     This default to nil.
     */ // :3
    CCNode* getStencil() const; // :3
    void setStencil(CCNode *pStencil); // :3

    /** The alpha threshold.
     The content is drawn only where the stencil have pixel with alpha greater than the alphaThreshold.
     Should be a float between 0 and 1.
     This default to 1 (so alpha test is disabled).
     */ // :3
    GLfloat getAlphaThreshold() const; // :3
    void setAlphaThreshold(GLfloat fAlphaThreshold); // :3

    /** Inverted. If this is set to YES,
     the stencil is inverted, so the content is drawn where the stencil is NOT drawn.
     This default to NO.
     */ // :3
    bool isInverted() const; // :3
    void setInverted(bool bInverted); // :3

    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCClippingNode, CCNode); // :3

protected: // :3
    CCClippingNode(); // :3
}; // :3

NS_CC_END // :3

#endif // __MISCNODE_CCCLIPPING_NODE_H__ :3
