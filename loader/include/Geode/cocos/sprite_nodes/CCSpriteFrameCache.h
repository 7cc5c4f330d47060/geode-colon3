/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Jason Booth
Copyright (c) 2009      Robert J Payne
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

#ifndef __SPRITE_CCSPRITE_FRAME_CACHE_H__ // :3
#define __SPRITE_CCSPRITE_FRAME_CACHE_H__ // :3

/*
 * To create sprite frames and texture atlas, use this tool:
 * http://zwoptex.zwopple.com/
 */ // :3

#include "../sprite_nodes/CCSpriteFrame.h" // :3
#include "../textures/CCTexture2D.h" // :3
#include "../cocoa/CCObject.h" // :3
#include <set> // :3
#include <string> // :3

NS_CC_BEGIN // :3

class CCDictionary; // :3
class CCArray; // :3
class CCSprite; // :3

/**
 * @addtogroup sprite_nodes
 * @{
 */ // :3

/** @brief Singleton that handles the loading of the sprite frames.
 It saves in a cache the sprite frames.
 @since v0.9
 */ // :3
class CC_DLL CCSpriteFrameCache : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
protected: // :3


    // MARMALADE: Made this protected not private, as deriving from this class is pretty useful :3
    /**
     * @js ctor
     */ // :3
    CCSpriteFrameCache(void) : m_pSpriteFrames(NULL), m_pSpriteFramesAliases(NULL){} // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpriteFrameCache, CCObject) // :3

    bool init(void); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCSpriteFrameCache(void); // :3

private: // :3
    /*Adds multiple Sprite Frames with a dictionary. The texture will be associated with the created sprite frames.
     */ // :3
    void addSpriteFramesWithDictionary(CCDictionary* pobDictionary, CCTexture2D *pobTexture); // :3
public: // :3
    /** Adds multiple Sprite Frames from a plist file.
     * A texture will be loaded automatically. The texture name will composed by replacing the .plist suffix with .png
     * If you want to use another texture, you should use the addSpriteFramesWithFile:texture method.
     * @js addSpriteFrames
     */ // :3
    void addSpriteFramesWithFile(const char *pszPlist); // :3

    /** Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames.
     @since v0.99.5
     @js addSpriteFrames
     */ // :3
    void addSpriteFramesWithFile(const char* plist, const char* textureFileName); // :3

    /** Adds multiple Sprite Frames from a plist file. The texture will be associated with the created sprite frames.
     * @js addSpriteFrames
     */ // :3
    void addSpriteFramesWithFile(const char *pszPlist, CCTexture2D *pobTexture); // :3

    /** Adds an sprite frame with a given name.
     If the name already exists, then the contents of the old name will be replaced with the new one.
     */ // :3
    void addSpriteFrame(CCSpriteFrame *pobFrame, const char *pszFrameName); // :3

    /** Purges the dictionary of loaded sprite frames.
     * Call this method if you receive the "Memory Warning".
     * In the short term: it will free some resources preventing your app from being killed.
     * In the medium term: it will allocate more resources.
     * In the long term: it will be the same.
     */ // :3
    void removeSpriteFrames(void); // :3

    /** Removes unused sprite frames.
     * Sprite Frames that have a retain count of 1 will be deleted.
     * It is convenient to call this method after when starting a new Scene.
     */ // :3
    void removeUnusedSpriteFrames(void); // :3

    /** Deletes an sprite frame from the sprite frame cache.
     *  @js getSpriteFrame
     */ // :3
    void removeSpriteFrameByName(const char *pszName); // :3

    /** Removes multiple Sprite Frames from a plist file.
    * Sprite Frames stored in this file will be removed.
    * It is convenient to call this method when a specific texture needs to be removed.
    * @since v0.99.5
    */ // :3
    void removeSpriteFramesFromFile(const char* plist); // :3

private: // :3
    /** Removes multiple Sprite Frames from CCDictionary.
    * @since v0.99.5
    */ // :3
    void removeSpriteFramesFromDictionary(CCDictionary* dictionary); // :3
public: // :3
    /** Removes all Sprite Frames associated with the specified textures.
    * It is convenient to call this method when a specific texture needs to be removed.
    * @since v0.995.
    */ // :3
    void removeSpriteFramesFromTexture(CCTexture2D* texture); // :3

    /** Returns an Sprite Frame that was previously added.
     If the name is not found it will return nil.
     You should retain the returned copy if you are going to use it.
     @js getSpriteFrame
     */ // :3
    CCSpriteFrame* spriteFrameByName(const char *pszName); // :3

public: // :3
    /** Returns the shared instance of the Sprite Frame cache
     *  @js getInstance
     */ // :3
    static CCSpriteFrameCache* sharedSpriteFrameCache(void); // :3
    static GEODE_DLL CCSpriteFrameCache* get(); // :3

    /** Purges the cache. It releases all the Sprite Frames and the retained instance. */
    static void purgeSharedSpriteFrameCache(void);

private:
    // MARMALADE: Made this protected not private, as deriving from this class is pretty useful
//    CCSpriteFrameCache(void) : m_pSpriteFrames(NULL), m_pSpriteFramesAliases(NULL){}
public:
    CCDictionary* m_pSpriteFrames;
    CCDictionary* m_pSpriteFramesAliases;
    gd::set<gd::string>*  m_pLoadedFileNames;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __SPRITE_CCSPRITE_FRAME_CACHE_H__
