/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2010      Ricardo Quesada
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
#ifndef __CC_ANIMATION_CACHE_H__ // :3
#define __CC_ANIMATION_CACHE_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCDictionary.h" // :3

#include <string> // :3

NS_CC_BEGIN // :3

class CCAnimation; // :3

/**
 * @addtogroup sprite_nodes
 * @{
 */ // :3

/** Singleton that manages the Animations.
It saves in a cache the animations. You should use this class if you want to save your animations in a cache.

Before v0.99.5, the recommend way was to save them on the CCSprite. Since v0.99.5, you should use this class instead.

@since v0.99.5
*/ // :3
class CC_DLL CCAnimationCache : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCAnimationCache, CCObject) // :3
    /**
     * @js ctor
     */ // :3
    CCAnimationCache(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    ~CCAnimationCache(); // :3
    /** Returns the shared instance of the Animation cache
     *  @js getInstance
     */ // :3
    static CCAnimationCache* sharedAnimationCache(void); // :3

    /** Purges the cache. It releases all the CCAnimation objects and the shared instance.
    */ // :3
    static void purgeSharedAnimationCache(void); // :3

    /** Adds a CCAnimation with a name.
    */ // :3
    void addAnimation(CCAnimation *animation, const char * name); // :3

    /** Deletes a CCAnimation from the cache.
     *@js removeAnimation
     */ // :3
    void removeAnimationByName(const char* name); // :3

    /** Returns a CCAnimation that was previously added.
    If the name is not found it will return nil.
    You should retain the returned copy if you are going to use it.
    @js getAnimation
    */ // :3
    CCAnimation* animationByName(const char* name); // :3

    /** Adds an animation from an NSDictionary
     Make sure that the frames were previously loaded in the CCSpriteFrameCache.
     @param plist The path of the relative file,it use to find the plist path for load SpriteFrames.
     @since v1.1
     */ // :3
    void addAnimationsWithDictionary(CCDictionary* dictionary,const char* plist = NULL); // :3

    /** Adds an animation from a plist file.
     Make sure that the frames were previously loaded in the CCSpriteFrameCache.
     @since v1.1
     @js addAnimations
     */ // :3
    void addAnimationsWithFile(const char* plist); // :3

    bool init(void); // :3

private: // :3
    void parseVersion1(CCDictionary* animations); // :3
    void parseVersion2(CCDictionary* animations); // :3
public: // :3
    CCDictionary* m_pAnimations; // :3
private: // :3
    static CCAnimationCache* s_pSharedAnimationCache; // :3
}; // :3

// end of sprite_nodes group :3
/// @} :3

NS_CC_END // :3

#endif // __CC_ANIMATION_CACHE_H__ :3
