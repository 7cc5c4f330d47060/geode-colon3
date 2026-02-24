/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __SPITE_NODE_CCSPRITE_H__ // :3
#define __SPITE_NODE_CCSPRITE_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../textures/CCTextureAtlas.h" // :3
#include "../include/ccTypes.h" // :3
#include "../cocoa/CCDictionary.h" // :3
#include <string> // :3
#ifdef EMSCRIPTEN // :3
#include "../base_nodes/CCGLBufferedNode.h" // :3
#endif // EMSCRIPTEN :3

NS_CC_BEGIN // :3

class CCSpriteBatchNode; // :3
class CCSpriteFrame; // :3
class CCAnimation; // :3
class CCRect; // :3
class CCPoint; // :3
class CCSize; // :3
class CCTexture2D; // :3
struct transformValues_; // :3

/**
 * @addtogroup sprite_nodes
 * @{
 */ // :3

#define CCSpriteIndexNotInitialized 0xffffffff     /// CCSprite invalid index on the CCSpriteBatchNode :3


/**
 * CCSprite is a 2d image ( http://en.wikipedia.org/wiki/Sprite_(computer_graphics) )
 *
 * CCSprite can be created with an image, or with a sub-rectangle of an image.
 *
 * If the parent or any of its ancestors is a CCSpriteBatchNode then the following features/limitations are valid
 *    - Features when the parent is a CCBatchNode:
 *        - MUCH faster rendering, specially if the CCSpriteBatchNode has many children. All the children will be drawn in a single batch.
 *
 *    - Limitations
 *        - Camera is not supported yet (eg: CCOrbitCamera action doesn't work)
 *        - GridBase actions are not supported (eg: CCLens, CCRipple, CCTwirl)
 *        - The Alias/Antialias property belongs to CCSpriteBatchNode, so you can't individually set the aliased property.
 *        - The Blending function property belongs to CCSpriteBatchNode, so you can't individually set the blending function property.
 *        - Parallax scroller is not supported, but can be simulated with a "proxy" sprite.
 *
 *  If the parent is an standard CCNode, then CCSprite behaves like any other CCNode:
 *    - It supports blending functions
 *    - It supports aliasing / antialiasing
 *    - But the rendering will be slower: 1 draw per children.
 *
 * The default anchorPoint in CCSprite is (0.5, 0.5).
 */ // :3
class CC_DLL CCSprite : public CCNodeRGBA, public CCTextureProtocol // :3
#ifdef EMSCRIPTEN // :3
, public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /// @{ :3
    /// @name Creators :3

    /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @return An empty sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* create(); // :3

    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   pszFileName The string which indicates a path to image file, e.g., "scene1/monster.png".
     * @return  A valid sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* create(const char *pszFileName); // :3

    /**
     * Creates a sprite with an image filename and a rect.
     *
     * @param   pszFileName The string wich indicates a path to image file, e.g., "scene1/monster.png"
     * @param   rect        Only the contents inside rect of pszFileName's texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* create(const char *pszFileName, const CCRect& rect); // :3

    /**
     * Creates a sprite with an exsiting texture contained in a CCTexture2D object
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   pTexture    A pointer to a CCTexture2D object.
     * @return  A valid sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* createWithTexture(CCTexture2D *pTexture); // :3

    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect); // :3

    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   pSpriteFrame    A sprite frame which involves a texture and a rect
     * @return  A valid sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame); // :3

    /**
     * Creates a sprite with an sprite frame name.
     *
     * A CCSpriteFrame will be fetched from the CCSpriteFrameCache by pszSpriteFrameName param.
     * If the CCSpriteFrame doesn't exist it will raise an exception.
     *
     * @param   pszSpriteFrameName A null terminated string which indicates the sprite frame name.
     * @return  A valid sprite object that is marked as autoreleased.
     */ // :3
    static CCSprite* createWithSpriteFrameName(const char *pszSpriteFrameName); // :3

    /// @}  end of creators group :3



    /// @{ :3
    /// @name Initializers :3

    /**
     * Default constructor
     * @js ctor
     */ // :3
    CCSprite(void); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSprite, CCNodeRGBA) // :3

    /**
     * Default destructor
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCSprite(void); // :3

    /**
     * Initializes an empty sprite with nothing init.
     */ // :3
    virtual bool init(void); // :3

    /**
     * Initializes a sprite with a texture.
     *
     * After initialization, the rect used will be the size of the texture, and the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @return  true if the sprite is initialized properly, false otherwise.
     */ // :3
    virtual bool initWithTexture(CCTexture2D *pTexture); // :3

    /**
     * Initializes a sprite with a texture and a rect.
     *
     * After initialization, the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an exisiting CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside rect of this texture will be applied for this sprite.
     * @return  true if the sprite is initialized properly, false otherwise.
     */ // :3
    virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect); // :3

    /**
     * Initializes a sprite with a texture and a rect in points, optionally rotated.
     *
     * After initialization, the offset will be (0,0).
     * @note    This is the designated initializer.
     *
     * @param   pTexture    A CCTexture2D object whose texture will be applied to this sprite.
     * @param   rect        A rectangle assigned the contents of texture.
     * @param   rotated     Whether or not the texture rectangle is rotated.
     * @return  true if the sprite is initialized properly, false otherwise.
     */ // :3
    virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated); // :3

    /**
     * Initializes a sprite with an SpriteFrame. The texture and rect in SpriteFrame will be applied on this sprite
     *
     * @param   pSpriteFrame  A CCSpriteFrame object. It should includes a valid texture and a rect
     * @return  true if the sprite is initialized properly, false otherwise.
     */ // :3
    virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame); // :3

    /**
     * Initializes a sprite with an sprite frame name.
     *
     * A CCSpriteFrame will be fetched from the CCSpriteFrameCache by name.
     * If the CCSpriteFrame doesn't exist it will raise an exception.
     *
     * @param   pszSpriteFrameName  A key string that can fected a volid CCSpriteFrame from CCSpriteFrameCache
     * @return  true if the sprite is initialized properly, false otherwise.
     */ // :3
    virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName); // :3

    /**
     * Initializes a sprite with an image filename.
     *
     * This method will find pszFilename from local file system, load its content to CCTexture2D,
     * then use CCTexture2D to create a sprite.
     * After initialization, the rect used will be the size of the image. The offset will be (0,0).
     *
     * @param   pszFilename The path to an image file in local file system
     * @return  true if the sprite is initialized properly, false otherwise.
     * @js init
     */ // :3
    virtual bool initWithFile(const char *pszFilename); // :3

    /**
     * Initializes a sprite with an image filename, and a rect.
     *
     * This method will find pszFilename from local file system, load its content to CCTexture2D,
     * then use CCTexture2D to create a sprite.
     * After initialization, the offset will be (0,0).
     *
     * @param   pszFilename The path to an image file in local file system.
     * @param   rect        The rectangle assigned the content area from texture.
     * @return  true if the sprite is initialized properly, false otherwise.
     * @js init
     */ // :3
    virtual bool initWithFile(const char *pszFilename, const CCRect& rect); // :3

    /// @} end of initializers :3


    /// @{ :3
    /// @name Functions inherited from CCTextureProtocol :3
    virtual void setTexture(CCTexture2D *texture); // :3
    virtual CCTexture2D* getTexture(void); // :3
    inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; } // :3
    /**
     * @js NA
     */ // :3
    inline ccBlendFunc getBlendFunc(void) { return m_sBlendFunc; } // :3
    /// @} :3

    // @note RobTop Addition :3
    virtual void setChildColor(const ccColor3B&); // :3
    // @note RobTop Addition :3
    virtual void setChildOpacity(GLubyte); // :3

    /// @{ :3
    /// @name Functions inherited from CCNode :3
    virtual void setScaleX(float fScaleX); // :3
    virtual void setScaleY(float fScaleY); // :3
    /**
     * @lua NA
     */ // :3
    virtual void setPosition(const CCPoint& pos); // :3
    virtual void setRotation(float fRotation); // :3
    virtual void setRotationX(float fRotationX); // :3
    virtual void setRotationY(float fRotationY); // :3
    virtual void setSkewX(float sx); // :3
    virtual void setSkewY(float sy); // :3
    virtual void removeChild(CCNode* pChild, bool bCleanup); // :3
    virtual void removeAllChildrenWithCleanup(bool bCleanup); // :3
    virtual void reorderChild(CCNode *pChild, int zOrder); // :3
    virtual void addChild(CCNode *pChild); // :3
    virtual void addChild(CCNode *pChild, int zOrder); // :3
    virtual void addChild(CCNode *pChild, int zOrder, int tag); // :3
    virtual void sortAllChildren(); // :3
    virtual void setScale(float fScale); // :3
    virtual void setVertexZ(float fVertexZ); // :3
    virtual void setAnchorPoint(const CCPoint& anchor); // :3
    virtual void ignoreAnchorPointForPosition(bool value); // :3
    virtual void setVisible(bool bVisible); // :3
    virtual void draw(void); // :3
    /// @} :3

    /// @{ :3
    /// @name Functions inherited from CCNodeRGBA :3
    virtual void setColor(const ccColor3B& color3); // :3
    virtual void updateDisplayedColor(const ccColor3B& parentColor); // :3
    virtual void setOpacity(GLubyte opacity); // :3
    virtual void setOpacityModifyRGB(bool modify); // :3
    virtual bool isOpacityModifyRGB(void); // :3
    virtual void updateDisplayedOpacity(GLubyte parentOpacity); // :3
    /// @} :3


    /// @{ :3
    /// @name BatchNode methods :3

    /**
     * Updates the quad according the rotation, position, scale values.
     */ // :3
    virtual void updateTransform(void); // :3

    /**
     * Returns the batch node object if this sprite is rendered by CCSpriteBatchNode
     *
     * @return The CCSpriteBatchNode object if this sprite is rendered by CCSpriteBatchNode,
     *         NULL if the sprite isn't used batch node.
     */ // :3
    virtual CCSpriteBatchNode* getBatchNode(void); // :3
    /**
     * Sets the batch node to sprite
     * @warning This method is not recommended for game developers. Sample code for using batch node
     * @code
     * CCSpriteBatchNode *batch = CCSpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
     * CCSprite *sprite = CCSprite::createWithTexture(batch->getTexture(), CCRectMake(0, 0, 57, 57));
     * batch->addChild(sprite);
     * layer->addChild(batch);
     * @endcode
     */ // :3
    virtual void setBatchNode(CCSpriteBatchNode *pobSpriteBatchNode); // :3

    /// @} end of BatchNode methods :3

    // @note RobTop Addition :3
    virtual void refreshTextureRect(void); // :3

    /// @{ :3
    /// @name Texture methods :3

    /**
     * Updates the texture rect of the CCSprite in points.
     * It will call setTextureRect:rotated:untrimmedSize with rotated = NO, and utrimmedSize = rect.size.
     */ // :3
    virtual void setTextureRect(const CCRect& rect); // :3

    /**
     * Sets the texture rect, rectRotated and untrimmed size of the CCSprite in points.
     * It will update the texture coordinates and the vertex rectangle.
     */ // :3
    virtual void setTextureRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize); // :3

    /**
     * Sets the vertex rect.
     * It will be called internally by setTextureRect.
     * Useful if you want to create 2x images from SD images in Retina Display.
     * Do not call it manually. Use setTextureRect instead.
     */ // :3
    virtual void setVertexRect(const CCRect& rect); // :3

    /// @} end of texture methods :3



    /// @{ :3
    /// @name Frames methods :3

    /**
     * Sets a new display frame to the CCSprite.
     */ // :3
    virtual void setDisplayFrame(CCSpriteFrame *pNewFrame); // :3

    /**
     * Returns whether or not a CCSpriteFrame is being displayed
     */ // :3
    virtual bool isFrameDisplayed(CCSpriteFrame *pFrame); // :3

    /**
     * Returns the current displayed frame.
     * @js NA
     */ // :3
    virtual CCSpriteFrame* displayFrame(void); // :3

    /// @} End of frames methods :3


    /// @{ :3
    /// @name Animation methods :3
    /**
     * Changes the display frame with animation name and index.
     * The animation name will be get from the CCAnimationCache
     */ // :3
    virtual void setDisplayFrameWithAnimationName(const char *animationName, int frameIndex); // :3
    /// @} :3


    /// @{ :3
    /// @name Sprite Properties' setter/getters :3

    /**
     * Whether or not the Sprite needs to be updated in the Atlas.
     *
     * @return true if the sprite needs to be updated in the Atlas, false otherwise.
     */ // :3
    inline virtual bool isDirty(void) { return m_bDirty; } // :3

    /**
     * Makes the Sprite to be updated in the Atlas.
     */ // :3
    inline virtual void setDirty(bool bDirty) { m_bDirty = bDirty; } // :3

    /**
     * Returns the quad (tex coords, vertex coords and color) information.
     * @js NA
     */ // :3
    inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; } // :3

    /**
     * Returns whether or not the texture rectangle is rotated.
     */ // :3
    inline bool isTextureRectRotated(void) { return m_bRectRotated; } // :3

    /**
     * Returns the index used on the TextureAtlas.
     */ // :3
    inline unsigned int getAtlasIndex(void) { return m_uAtlasIndex; } // :3

    /**
     * Sets the index used on the TextureAtlas.
     * @warning Don't modify this value unless you know what you are doing
     */ // :3
    inline void setAtlasIndex(unsigned int uAtlasIndex) { m_uAtlasIndex = uAtlasIndex; } // :3

    /**
     * Returns the rect of the CCSprite in points
     */ // :3
    inline const CCRect& getTextureRect(void) { return m_obRect; } // :3

    /**
     * Gets the weak reference of the CCTextureAtlas when the sprite is rendered using via CCSpriteBatchNode
     */ // :3
    inline CCTextureAtlas* getTextureAtlas(void) { return m_pobTextureAtlas; } // :3

    /**
     * Sets the weak reference of the CCTextureAtlas when the sprite is rendered using via CCSpriteBatchNode
     */ // :3
    inline void setTextureAtlas(CCTextureAtlas *pobTextureAtlas) { m_pobTextureAtlas = pobTextureAtlas; } // :3

    /**
     * Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
     */ // :3
    inline const CCPoint& getOffsetPosition(void) { return m_obOffsetPosition; } // :3


    /**
     * Returns the flag which indicates whether the sprite is flipped horizontally or not.
     *
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleX(sprite->getScaleX() * -1);
     *
     * @return true if the sprite is flipped horizaontally, false otherwise.
     * @js isFlippedX
     */ // :3
    bool isFlipX(void); // :3
    /**
     * Sets whether the sprite should be flipped horizontally or not.
     *
     * @param bFlipX true if the sprite should be flipped horizaontally, false otherwise.
     */ // :3
    void setFlipX(bool bFlipX); // :3

    /**
     * Return the flag which indicates whether the sprite is flipped vertically or not.
     *
     * It only flips the texture of the sprite, and not the texture of the sprite's children.
     * Also, flipping the texture doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleY(sprite->getScaleY() * -1);
     *
     * @return true if the sprite is flipped vertically, flase otherwise.
     * @js isFlippedY
     */ // :3
    bool isFlipY(void); // :3
    /**
     * Sets whether the sprite should be flipped vertically or not.
     *
     * @param bFlipY true if the sprite should be flipped vertically, flase otherwise.
     */ // :3
    void setFlipY(bool bFlipY); // :3

    /// @} End of Sprite properties getter/setters :3

	inline cocos2d::CCPoint const& getUnflippedOffsetPosition() { // :3
        return m_obUnflippedOffsetPositionFromCenter; // :3
    } // :3


protected: // :3
    void updateColor(void); // :3
    // @note RobTop Addition: Changed the param from CCRect to const CCRect& :3
    virtual void setTextureCoords(const CCRect& rect); // :3
    virtual void updateBlendFunc(void); // :3
    virtual void setReorderChildDirtyRecursively(void); // :3
    virtual void setDirtyRecursively(bool bValue); // :3

public: // :3
    // :3
    // Data used when the sprite is rendered using a CCSpriteSheet :3
    // :3
    CCTextureAtlas*     m_pobTextureAtlas;      /// CCSpriteBatchNode texture atlas (weak reference) :3
    unsigned int        m_uAtlasIndex;          /// Absolute (real) Index on the SpriteSheet :3
    CCSpriteBatchNode*  m_pobBatchNode;         /// Used batch node (weak reference) :3

    bool                m_bDirty;               /// Whether the sprite needs to be updated :3
    bool                m_bRecursiveDirty;      /// Whether all of the sprite's children needs to be updated :3
    bool                m_bHasChildren;         /// Whether the sprite contains children :3
    bool                m_bShouldBeHidden;      /// should not be drawn because one of the ancestors is not visible :3
    CCAffineTransform   m_transformToBatch; // :3

    // :3
    // Data used when the sprite is self-rendered :3
    // :3
    ccBlendFunc        m_sBlendFunc;            /// It's required for CCTextureProtocol inheritance :3
    CCTexture2D*       m_pobTexture;            /// CCTexture2D object that is used to render the sprite :3

    // :3
    // Shared data :3
    // :3

    // texture :3
    CCRect m_obRect;                            /// Retangle of CCTexture2D :3
    bool   m_bRectRotated;                      /// Whether the texture is rotated :3

    // Offset Position (used by Zwoptex) :3
    CCPoint m_obOffsetPosition; // :3
    CCPoint m_obUnflippedOffsetPositionFromCenter; // :3

    // vertex coords, texture coords and color info :3
    ccV3F_C4B_T2F_Quad m_sQuad; // :3

    // opacity and RGB protocol :3
    bool m_bOpacityModifyRGB; // :3

    // image is flipped :3
    bool m_bFlipX;                              /// Whether the sprite is flipped horizaontally or not. :3
    bool m_bFlipY;                              /// Whether the sprite is flipped vertically or not. :3

    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(bool, m_bDontDraw, DontDraw); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(bool, m_bUseVertexMod, UseVertexMod); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(float, m_fTlVertexMod, TlVertexMod); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(float, m_fTrVertexMod, TrVertexMod); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(float, m_fBlVertexMod, BlVertexMod); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(float, m_fBrVertexMod, BrVertexMod); // :3
    // @note RobTop Addition :3
    float m_fTextureLeft; // :3
    // @note RobTop Addition :3
    float m_fTextureRight; // :3
    // @note RobTop Addition :3
    float m_fTextureBottom; // :3
    // @note RobTop Addition :3
    float m_fTextureTop; // :3
}; // :3


// end of sprite_nodes group :3
/// @} :3

NS_CC_END // :3

#endif // __SPITE_NODE_CCSPRITE_H__ :3
