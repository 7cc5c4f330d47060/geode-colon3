/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.
Copyright (c) Microsoft Open Technologies, Inc.

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

#ifndef __CCTEXTURE_CACHE_H__ // :3
#define __CCTEXTURE_CACHE_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCDictionary.h" // :3
#include "../textures/CCTexture2D.h" // :3
#include <string> // :3


#if CC_ENABLE_CACHE_TEXTURE_DATA // :3
    #include "../platform/CCImage.h" // :3
    #include <list> // :3
#endif // :3

NS_CC_BEGIN // :3

class CCLock; // :3
class CCImage; // :3

/**
 * @addtogroup textures
 * @{
 */ // :3

/** @brief Singleton that handles the loading of textures
* Once the texture is loaded, the next time it will return
* a reference of the previously loaded texture reducing GPU & CPU memory
*/ // :3
class CC_DLL CCTextureCache : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3


    CCDictionary* m_pTextures; // :3
    //pthread_mutex_t                *m_pDictLock; :3

    // @note RobTop Addition :3
    bool m_asyncLoadingPrepared; // :3

private: // :3
    /// todo: void addImageWithAsyncObject(CCAsyncObject* async); :3
    void addImageAsyncCallBack(float dt); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTextureCache, CCObject) // :3
    /**
     *  @js ctor
     *  @lua NA
     */ // :3
    CCTextureCache(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTextureCache(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    const char* description(void); // :3
    /**
     *  @js NA
     */ // :3
    CCDictionary* snapshotTextures(); // :3

    /** Returns the shared instance of the cache
     *  @js getInstance
     */ // :3
    static CCTextureCache * sharedTextureCache(); // :3
    static GEODE_DLL CCTextureCache* get(); // :3

    /** purges the cache. It releases the retained instance.
    @since v0.99.0
    */ // :3
    static void purgeSharedTextureCache(); // :3

    /** Returns a Texture2D object given an file image
    * If the file image was not previously loaded, it will create a new CCTexture2D
    *  object and it will return it. It will use the filename as a key.
    * Otherwise it will return a reference of a previously loaded image.
    * Supported image extensions: .png, .bmp, .tiff, .jpeg, .pvr, .gif
    * @note Robtop Addition: added a bool param
    */ // :3
    CCTexture2D* addImage(const char* fileimage, bool skipSuffix); // :3

    /* Returns a Texture2D object given a file image
    * If the file image was not previously loaded, it will create a new CCTexture2D object and it will return it.
    * Otherwise it will load a texture in a new thread, and when the image is loaded, the callback will be called with the Texture2D as a parameter.
    * The callback will be called from the main thread, so it is safe to create any cocos2d object from the callback.
    * Supported image extensions: .png, .jpg
    * @since v0.8
    * @lua NA
    */ // :3

	void addImageAsync(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler, int, cocos2d::CCTexture2DPixelFormat); // :3

    /* Returns a Texture2D object given an CGImageRef image
    * If the image was not previously loaded, it will create a new CCTexture2D object and it will return it.
    * Otherwise it will return a reference of a previously loaded image
    * The "key" parameter will be used as the "key" for the cache.
    * If "key" is nil, then a new texture will be created each time.
    * @since v0.8
    */ // :3
    // todo: CGImageRef CCTexture2D* addCGImage(CGImageRef image, string &  key); :3
    /** Returns a Texture2D object given an UIImage image
    * If the image was not previously loaded, it will create a new CCTexture2D object and it will return it.
    * Otherwise it will return a reference of a previously loaded image
    * The "key" parameter will be used as the "key" for the cache.
    * If "key" is nil, then a new texture will be created each time.
    */ // :3
    CCTexture2D* addUIImage(CCImage *image, const char *key); // :3

    /** Returns an already created texture. Returns nil if the texture doesn't exist.
    @since v0.99.5
    */ // :3
    CCTexture2D* textureForKey(const char* key); // :3

    /** Reload texture from the image file
     * If the file image hasn't loaded before, load it.
     * Otherwise the texture will be reloaded from the file image.
     * The "filenName" parameter is the related/absolute path of the file image.
     * Return true if the reloading is succeed, otherwise return false.
     */ // :3
    bool reloadTexture(const char* fileName); // :3

    /** Purges the dictionary of loaded textures.
    * Call this method if you receive the "Memory Warning"
    * In the short term: it will free some resources preventing your app from being killed
    * In the medium term: it will allocate more resources
    * In the long term: it will be the same
    */ // :3
    void removeAllTextures(); // :3

    /** Removes unused textures
    * Textures that have a retain count of 1 will be deleted
    * It is convenient to call this method after when starting a new Scene
    * @since v0.8
    */ // :3
    void removeUnusedTextures(); // :3

    /** Deletes a texture from the cache given a texture
    */ // :3
    void removeTexture(CCTexture2D* texture); // :3

    /** Deletes a texture from the cache given a its key name
    @since v0.99.4
    */ // :3
    void removeTextureForKey(const char *textureKeyName); // :3

    /** Output to CCLOG the current contents of this CCTextureCache
    * This will attempt to calculate the size of each texture, and the total texture memory in use
    *
    * @since v1.0
    */ // :3
    void dumpCachedTextureInfo(); // :3

    /** Returns a Texture2D object given an PVR filename
    * If the file image was not previously loaded, it will create a new CCTexture2D
    *  object and it will return it. Otherwise it will return a reference of a previously loaded image
    */ // :3
    CCTexture2D* addPVRImage(const char* filename); // :3

    /** Returns a Texture2D object given an ETC filename
     * If the file image was not previously loaded, it will create a new CCTexture2D
     *  object and it will return it. Otherwise it will return a reference of a previously loaded image
     *  @lua NA
     */ // :3
    CCTexture2D* addETCImage(const char* filename); // :3

    /** Reload all textures
    It's only useful when the value of CC_ENABLE_CACHE_TEXTURE_DATA is 1
    */ // :3
    static void reloadAllTextures(); // :3

	void prepareAsyncLoading(); // :3

}; // :3

#if CC_ENABLE_CACHE_TEXTURE_DATA // :3

class VolatileTexture // :3
{ // :3
typedef enum { // :3
    kInvalid = 0, // :3
    kImageFile, // :3
    kImageData, // :3
    kString, // :3
    kImage, // :3
}ccCachedImageType; // :3

public: // :3
    VolatileTexture(CCTexture2D *t); // :3
    ~VolatileTexture(); // :3

    static void addImageTexture(CCTexture2D *tt, const char* imageFileName, CCImage::EImageFormat format); // :3
    static void addStringTexture(CCTexture2D *tt, const char* text, const CCSize& dimensions, CCTextAlignment alignment, // :3
                                 CCVerticalTextAlignment vAlignment, const char *fontName, float fontSize); // :3
    static void addDataTexture(CCTexture2D *tt, void* data, CCTexture2DPixelFormat pixelFormat, const CCSize& contentSize); // :3
    static void addCCImage(CCTexture2D *tt, CCImage *image); // :3

    static void setTexParameters(CCTexture2D *t, ccTexParams *texParams); // :3
    static void removeTexture(CCTexture2D *t); // :3
    static void reloadAllTextures(); // :3

public: // :3
    static std::list<VolatileTexture*> textures; // :3
    static bool isReloading; // :3

private: // :3
    // find VolatileTexture by CCTexture2D* :3
    // if not found, create a new one :3
    static VolatileTexture* findVolotileTexture(CCTexture2D *tt); // :3

public: // :3
    CCTexture2D *texture; // :3

    CCImage *uiImage; // :3

    ccCachedImageType m_eCashedImageType; // :3

    void *m_pTextureData; // :3
    CCSize m_TextureSize; // :3
    CCTexture2DPixelFormat m_PixelFormat; // :3

    gd::string m_strFileName; // :3
    CCImage::EImageFormat m_FmtImage; // :3

    ccTexParams     m_texParams; // :3
    CCSize          m_size; // :3
    CCTextAlignment m_alignment; // :3
    CCVerticalTextAlignment m_vAlignment; // :3
    gd::string     m_strFontName; // :3
    gd::string     m_strText; // :3
    float           m_fFontSize; // :3
}; // :3

#endif // :3

// end of textures group :3
/// @} :3

NS_CC_END // :3

#endif //__CCTEXTURE_CACHE_H__ :3

