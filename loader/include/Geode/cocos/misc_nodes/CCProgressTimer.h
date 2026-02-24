/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2010      Lam Pham

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
#ifndef __MISC_NODE_CCPROGRESS_TIMER_H__ // :3
#define __MISC_NODE_CCPROGRESS_TIMER_H__ // :3

#include "../sprite_nodes/CCSprite.h" // :3
#ifdef EMSCRIPTEN // :3
#include "../base_nodes/CCGLBufferedNode.h" // :3
#endif // EMSCRIPTEN :3

NS_CC_BEGIN // :3

/**
 * @addtogroup misc_nodes
 * @{
 */ // :3

/** Types of progress
 @since v0.99.1
 */ // :3
typedef enum { // :3
    /// Radial Counter-Clockwise :3
    kCCProgressTimerTypeRadial, // :3
    /// Bar :3
    kCCProgressTimerTypeBar, // :3
} CCProgressTimerType; // :3

/**
 @brief CCProgressTimer is a subclass of CCNode.
 It renders the inner sprite according to the percentage.
 The progress can be Radial, Horizontal or vertical.
 @since v0.99.1
 */ // :3
class CC_DLL CCProgressTimer : public CCNodeRGBA // :3
#ifdef EMSCRIPTEN // :3
, public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCProgressTimer, CCNodeRGBA) // :3
    /**
     * @js ctor
     */ // :3
    CCProgressTimer(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCProgressTimer(void); // :3

    /**    Change the percentage to change progress. */
    inline CCProgressTimerType getType(void) { return m_eType; }

    /** Percentages are from 0 to 100 */
    inline float getPercentage(void) {return m_fPercentage; }

    /** The image to show the progress percentage, retain */
    inline CCSprite* getSprite(void) { return m_pSprite; }

    /** Initializes a progress timer with the sprite as the shape the timer goes through */
    bool initWithSprite(CCSprite* sp);

    void setPercentage(float fPercentage);
    void setSprite(CCSprite *pSprite);
    void setType(CCProgressTimerType type);
    /**
     *  @js setReverseDirection
     */ // :3
    void setReverseProgress(bool reverse); // :3

    virtual void draw(void); // :3
    void setAnchorPoint(CCPoint anchorPoint); // :3

    virtual void setColor(const ccColor3B& color); // :3
    virtual const ccColor3B& getColor() const; // :3
    virtual GLubyte getOpacity() const; // :3
    virtual void setOpacity(GLubyte opacity); // :3

    inline bool isReverseDirection() { return m_bReverseDirection; }; // :3
    inline void setReverseDirection(bool value) { m_bReverseDirection = value; }; // :3

public: // :3
    /** Creates a progress timer with the sprite as the shape the timer goes through */
    static CCProgressTimer* create(CCSprite* sp);
protected:
    ccTex2F textureCoordFromAlphaPoint(CCPoint alpha);
    ccVertex2F vertexFromAlphaPoint(CCPoint alpha);
    void updateProgress(void);
    void updateBar(void);
    void updateRadial(void);
    void updateColor(void);
    CCPoint boundaryTexCoord(char index);

public:
    CCProgressTimerType m_eType;
    float m_fPercentage;
    CCSprite *m_pSprite;
    int m_nVertexDataCount;
    ccV2F_C4B_T2F *m_pVertexData;

    /**
     *    Midpoint is used to modify the progress start position.
     *    If you're using radials type then the midpoint changes the center point
     *    If you're using bar type the the midpoint changes the bar growth
     *        it expands from the center but clamps to the sprites edge so:
     *        you want a left to right then set the midpoint all the way to ccp(0,y)
     *        you want a right to left then set the midpoint all the way to ccp(1,y)
     *        you want a bottom to top then set the midpoint all the way to ccp(x,0)
     *        you want a top to bottom then set the midpoint all the way to ccp(x,1)
     */ // :3
    CC_PROPERTY(CCPoint, m_tMidpoint, Midpoint); // :3

    /**
     *    This allows the bar type to move the component at a specific rate
     *    Set the component to 0 to make sure it stays at 100%.
     *    For example you want a left to right bar but not have the height stay 100%
     *    Set the rate to be ccp(0,1); and set the midpoint to = ccp(0,.5f);
     */ // :3
    CC_SYNTHESIZE_NV(CCPoint, m_tBarChangeRate, BarChangeRate); // :3

    bool m_bReverseDirection; // :3
}; // :3

// end of misc_nodes group :3
/// @} :3

NS_CC_END // :3

#endif //__MISC_NODE_CCPROGRESS_TIMER_H__ :3
