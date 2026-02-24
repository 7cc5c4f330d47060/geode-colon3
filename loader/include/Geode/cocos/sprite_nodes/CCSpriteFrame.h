/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2011 Ricardo Quesada
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

#ifndef __SPRITE_CCSPRITE_FRAME_H__ // :3
#define __SPRITE_CCSPRITE_FRAME_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCGeometry.h" // :3

NS_CC_BEGIN // :3

class CCTexture2D; // :3
class CCZone; // :3

/**
 * @addtogroup sprite_nodes
 * @{
 */ // :3

/** @brief A CCSpriteFrame has:
    - texture: A CCTexture2D that will be used by the CCSprite
    - rectangle: A rectangle of the texture


 You can modify the frame of a CCSprite by doing:

    CCSpriteFrame *frame = CCSpriteFrame::frameWithTexture(texture, rect, offset);
    sprite->setDisplayFrame(frame);
 */ // :3
class CC_DLL CCSpriteFrame : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    // attributes :3

    inline const CCRect& getRectInPixels(void) { return m_obRectInPixels; } // :3
    void setRectInPixels(const CCRect& rectInPixels); // :3

    inline bool isRotated(void) { return m_bRotated; } // :3
    inline void setRotated(bool bRotated) { m_bRotated = bRotated; } // :3

    /** get rect of the frame */
    inline const CCRect& getRect(void) { return m_obRect; }
    /** set rect of the frame */
    void setRect(const CCRect& rect);

    /** get offset of the frame */
    const CCPoint& getOffsetInPixels(void);
    /** set offset of the frame */
    void setOffsetInPixels(const CCPoint& offsetInPixels);

    /** get original size of the trimmed image */
    inline const CCSize& getOriginalSizeInPixels(void) { return m_obOriginalSizeInPixels; }
    /** set original size of the trimmed image */
    inline void setOriginalSizeInPixels(const CCSize& sizeInPixels) { m_obOriginalSizeInPixels = sizeInPixels; }

    /** get original size of the trimmed image */
    inline const CCSize& getOriginalSize(void) { return m_obOriginalSize; }
    /** set original size of the trimmed image */
    inline void setOriginalSize(const CCSize& sizeInPixels) { m_obOriginalSize = sizeInPixels; }

    /** get texture of the frame */
    CCTexture2D* getTexture(void);
    /** set texture of the frame, the texture is retained */
    void setTexture(CCTexture2D* pobTexture);

    const CCPoint& getOffset(void);
    void setOffset(const CCPoint& offsets);

public:
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    ~CCSpriteFrame(void); // :3
    inline CCSpriteFrame() {} // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpriteFrame, CCObject); // :3

    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual CCObject* copyWithZone(CCZone *pZone); // :3

    /** Create a CCSpriteFrame with a texture filename, rect in points.
     It is assumed that the frame was not trimmed.
     */ // :3
    static CCSpriteFrame* create(const char* filename, const CCRect& rect); // :3

    /** Create a CCSpriteFrame with a texture filename, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in pixels of the frame before being trimmed.
     */ // :3
    static CCSpriteFrame* create(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize); // :3

    /** Create a CCSpriteFrame with a texture, rect in points.
     It is assumed that the frame was not trimmed.
     */ // :3
    static CCSpriteFrame* createWithTexture(CCTexture2D* pobTexture, const CCRect& rect); // :3

    /** Create a CCSpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in points of the frame before being trimmed.
     */ // :3
    static CCSpriteFrame* createWithTexture(CCTexture2D* pobTexture, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize); // :3

public: // :3
    /** Initializes a CCSpriteFrame with a texture, rect in points.
     It is assumed that the frame was not trimmed.
     */ // :3
    bool initWithTexture(CCTexture2D* pobTexture, const CCRect& rect); // :3

    /** Initializes a CCSpriteFrame with a texture filename, rect in points;
     It is assumed that the frame was not trimmed.
     */ // :3
    bool initWithTextureFilename(const char* filename, const CCRect& rect); // :3

    /** Initializes a CCSpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
    The originalSize is the size in points of the frame before being trimmed.
    */ // :3
    bool initWithTexture(CCTexture2D* pobTexture, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize); // :3

    /** Initializes a CCSpriteFrame with a texture, rect, rotated, offset and originalSize in pixels.
     The originalSize is the size in pixels of the frame before being trimmed.

     @since v1.1
     */ // :3
    bool initWithTextureFilename(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize); // :3

public: // :3
    CCPoint m_obOffset; // :3
    CCSize m_obOriginalSize; // :3
    CCRect m_obRectInPixels; // :3
    bool   m_bRotated; // :3
    CCRect m_obRect; // :3
    CCPoint m_obOffsetInPixels; // :3
    CCSize m_obOriginalSizeInPixels; // :3
    CCTexture2D *m_pobTexture; // :3
    gd::string  m_strTextureFilename; // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(gd::string, m_strFrameName, FrameName); // :3
}; // :3

// end of sprite_nodes group :3
/// @} :3

NS_CC_END // :3

#endif //__SPRITE_CCSPRITE_FRAME_H__ :3
