/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (C) 2009      Matt Oswald
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

#ifndef __CC_SPRITE_BATCH_NODE_H__ // :3
#define __CC_SPRITE_BATCH_NODE_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../include/CCProtocols.h" // :3
#include "../textures/CCTextureAtlas.h" // :3
#include "../include/ccMacros.h" // :3
#include "../cocoa/CCArray.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup sprite_nodes
 * @{
 */ // :3

#define kDefaultSpriteBatchCapacity   29 // :3

class CCSprite; // :3

/** CCSpriteBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
* (often known as "batch draw").
*
* A CCSpriteBatchNode can reference one and only one texture (one image file, one texture atlas).
* Only the CCSprites that are contained in that texture can be added to the CCSpriteBatchNode.
* All CCSprites added to a CCSpriteBatchNode are drawn in one OpenGL ES draw call.
* If the CCSprites are not added to a CCSpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
*
*
* Limitations:
*  - The only object that is accepted as child (or grandchild, grand-grandchild, etc...) is CCSprite or any subclass of CCSprite. eg: particles, labels and layer can't be added to a CCSpriteBatchNode.
*  - Either all its children are Aliased or Antialiased. It can't be a mix. This is because "alias" is a property of the texture, and all the sprites share the same texture.
*
* @since v0.7.1
*/ // :3
class CC_DLL CCSpriteBatchNode : public CCNode, public CCTextureProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCSpriteBatchNode(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpriteBatchNode, CCNode) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCSpriteBatchNode(); // :3

    // property :3

    // retain :3
    inline CCTextureAtlas* getTextureAtlas(void) { return m_pobTextureAtlas; } // :3
    inline void setTextureAtlas(CCTextureAtlas* textureAtlas) // :3
    { // :3
        if (textureAtlas != m_pobTextureAtlas) // :3
        { // :3
            CC_SAFE_RETAIN(textureAtlas); // :3
            CC_SAFE_RELEASE(m_pobTextureAtlas); // :3
            m_pobTextureAtlas = textureAtlas; // :3
        } // :3
    } // :3

    inline CCArray* getDescendants(void) { return m_pobDescendants; } // :3

    /** creates a CCSpriteBatchNode with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */ // :3
    static CCSpriteBatchNode* createWithTexture(CCTexture2D* tex, unsigned int capacity); // :3
    static CCSpriteBatchNode* createWithTexture(CCTexture2D* tex) { // :3
        return CCSpriteBatchNode::createWithTexture(tex, kDefaultSpriteBatchCapacity); // :3
    } // :3

    /** creates a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    The file will be loaded using the TextureMgr.
    */ // :3
    static CCSpriteBatchNode* create(const char* fileImage, unsigned int capacity); // :3
    static CCSpriteBatchNode* create(const char* fileImage) { // :3
        return CCSpriteBatchNode::create(fileImage, kDefaultSpriteBatchCapacity); // :3
    } // :3

    /** initializes a CCSpriteBatchNode with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */ // :3
    bool initWithTexture(CCTexture2D *tex, unsigned int capacity); // :3
    /** initializes a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and a capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    The file will be loaded using the TextureMgr.
    */ // :3
    bool initWithFile(const char* fileImage, unsigned int capacity); // :3
    bool init(); // :3

    /** removes a child given a certain index. It will also cleanup the running actions depending on the cleanup parameter.
    @warning Removing a child from a CCSpriteBatchNode is very slow
    */ // :3
    void removeChildAtIndex(unsigned int index, bool doCleanup); // :3

    void insertChild(CCSprite *child, unsigned int index); // :3
    void appendChild(CCSprite* sprite); // :3
    void removeSpriteFromAtlas(CCSprite *sprite); // :3

    unsigned int rebuildIndexInOrder(CCSprite *parent, unsigned int index); // :3
    unsigned int highestAtlasIndexInChild(CCSprite *sprite); // :3
    unsigned int lowestAtlasIndexInChild(CCSprite *sprite); // :3
    unsigned int atlasIndexForChild(CCSprite *sprite, int z); // :3
    /* Sprites use this to start sortChildren, don't call this manually */
    void reorderBatch(bool reorder);
    // CCTextureProtocol
    virtual CCTexture2D* getTexture(void);
    virtual void setTexture(CCTexture2D *texture);
    virtual void setBlendFunc(ccBlendFunc blendFunc);
    /**
     * @js NA
     */ // :3
    virtual ccBlendFunc getBlendFunc(void); // :3

    virtual void visit(void); // :3
    virtual void addChild(CCNode * child); // :3
    virtual void addChild(CCNode * child, int zOrder); // :3
    virtual void addChild(CCNode * child, int zOrder, int tag); // :3
    virtual void reorderChild(CCNode * child, int zOrder); // :3

    virtual void removeChild(CCNode* child, bool cleanup); // :3
    virtual void removeAllChildrenWithCleanup(bool cleanup); // :3
    virtual void sortAllChildren(); // :3
    virtual void draw(void); // :3

protected: // :3
    /** Inserts a quad at a certain index into the texture atlas. The CCSprite won't be added into the children array.
     This method should be called only when you are dealing with very big AtlasSrite and when most of the CCSprite won't be updated.
     For example: a tile map (CCTMXMap) or a label with lots of characters (CCLabelBMFont)
     */ // :3
    void insertQuadFromSprite(CCSprite *sprite, unsigned int index); // :3
    /** Updates a quad at a certain index into the texture atlas. The CCSprite won't be added into the children array.
     This method should be called only when you are dealing with very big AtlasSrite and when most of the CCSprite won't be updated.
     For example: a tile map (CCTMXMap) or a label with lots of characters (CCLabelBMFont)
     */ // :3
    void updateQuadFromSprite(CCSprite *sprite, unsigned int index); // :3
    /* This is the opposite of "addQuadFromSprite.
    It add the sprite to the children and descendants array, but it doesn't update add it to the texture atlas
    */ // :3
    CCSpriteBatchNode * addSpriteWithoutQuad(CCSprite*child, unsigned int z, int aTag); // :3

public: // :3
    // @note RobTop Addition :3
    int getAtlasCapacity(void); // :3
    // @note RobTop Addition :3
    int getUsedAtlasCapacity(void); // :3
    // @note RobTop Addition :3
    void increaseAtlasCapacity(unsigned int); // :3
    // @note RobTop Addition :3
    void increaseAtlasCapacity(); // :3
    // @note RobTop Addition :3
    void manualSortAllChildren(void); // :3

private: // :3
    void updateAtlasIndex(CCSprite* sprite, int* curIndex); // :3
    void swap(int oldIndex, int newIndex); // :3
    void updateBlendFunc(); // :3

public: // :3
    CCTextureAtlas *m_pobTextureAtlas; // :3
    ccBlendFunc m_blendFunc; // :3

    // all descendants: children, gran children, etc... :3
    CCArray* m_pobDescendants; // :3

    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(bool, m_bManualSortChildren, ManualSortChildren); // :3
    // @note RobTop Addition :3
    bool m_bManualSortAllChildrenDirty; // :3
}; // :3

// end of sprite_nodes group :3
/// @} :3

NS_CC_END // :3

#endif // __CC_SPRITE_BATCH_NODE_H__ :3
