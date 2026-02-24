/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (C) 2008      Apple Inc. All Rights Reserved.

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

#ifndef __CCTEXTURE2D_H__ // :3
#define __CCTEXTURE2D_H__ // :3

#include <string> // :3
#include "../cocoa/CCObject.h" // :3
#include "../cocoa/CCGeometry.h" // :3
#include "../include/ccTypes.h" // :3
#ifdef EMSCRIPTEN // :3
#include "../base_nodes/CCGLBufferedNode.h" // :3
#endif // EMSCRIPTEN :3

NS_CC_BEGIN // :3

class CCImage; // :3

/**
 * @addtogroup textures
 * @{
 */ // :3

//CONSTANTS: :3

/** @typedef CCTexture2DPixelFormat
Possible texture pixel formats
*/ // :3
typedef enum { // :3

    //! 32-bit texture: RGBA8888 :3
    kCCTexture2DPixelFormat_RGBA8888, // :3
    //! 24-bit texture: RGBA888 :3
    kCCTexture2DPixelFormat_RGB888, // :3
    //! 16-bit texture without Alpha channel :3
    kCCTexture2DPixelFormat_RGB565, // :3
    //! 8-bit textures used as masks :3
    kCCTexture2DPixelFormat_A8, // :3
    //! 8-bit intensity texture :3
    kCCTexture2DPixelFormat_I8, // :3
    //! 16-bit textures used as masks :3
    kCCTexture2DPixelFormat_AI88, // :3
    //! 16-bit textures: RGBA4444 :3
    kCCTexture2DPixelFormat_RGBA4444, // :3
    //! 16-bit textures: RGB5A1 :3
    kCCTexture2DPixelFormat_RGB5A1, // :3
    //! 4-bit PVRTC-compressed texture: PVRTC4 :3
    kCCTexture2DPixelFormat_PVRTC4, // :3
    //! 2-bit PVRTC-compressed texture: PVRTC2 :3
    kCCTexture2DPixelFormat_PVRTC2, // :3


    //! Default texture format: RGBA8888 :3
    kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888, // :3

    // backward compatibility stuff :3
    kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888, // :3
    kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888, // :3
    kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565, // :3
    kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8, // :3
    kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444, // :3
    kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1, // :3
    kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default // :3

} CCTexture2DPixelFormat; // :3

class CCGLProgram; // :3

/**
Extension to set the Min / Mag filter
*/ // :3
typedef struct _ccTexParams { // :3
    GLuint    minFilter; // :3
    GLuint    magFilter; // :3
    GLuint    wrapS; // :3
    GLuint    wrapT; // :3
} ccTexParams; // :3

//CLASS INTERFACES: :3

/** @brief CCTexture2D class.
* This class allows to easily create OpenGL 2D textures from images, text or raw data.
* The created CCTexture2D object will always have power-of-two dimensions.
* Depending on how you create the CCTexture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
* Be aware that the content of the generated textures will be upside-down!
*/ // :3
class CC_DLL CCTexture2D : public CCObject // :3
#ifdef EMSCRIPTEN // :3
, public CCGLBufferedNode // :3
#endif // EMSCRIPTEN :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCTexture2D(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTexture2D, CCObject) // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCTexture2D(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    const char* description(void); // :3

    /** These functions are needed to create mutable textures
     * @js NA
     */ // :3
    void releaseData(void *data); // :3
    /**
     * @js NA
     */ // :3
    void* keepData(void *data, unsigned int length); // :3

    /** Initializes with a texture2d with data
     * @js NA
     * @lua NA
     */ // :3
    bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize); // :3

    /**
    Drawing extensions to make it easy to draw basic quads using a CCTexture2D object.
    These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
    */ // :3
    /** draws a texture at a given point */
    void drawAtPoint(const CCPoint& point);
    /** draws a texture inside a rect */
    void drawInRect(const CCRect& rect);

    /**
    Extensions to make it easy to create a CCTexture2D object from an image file.
    Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
    */ // :3
    /** Initializes a texture from a UIImage object */

    bool initWithImage(CCImage * uiImage);

    /** Initializes a texture from a string with dimensions, alignment, font name and font size */
    bool initWithString(const char *text,  const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment);
    /** Initializes a texture from a string with font name and font size */
    bool initWithString(const char *text, const char *fontName, float fontSize);
    /** Initializes a texture from a string using a text definition*/
    bool initWithString(const char *text, ccFontDefinition *textDefinition);

    /** Initializes a texture from a PVR file */
    bool initWithPVRFile(const char* file);

    /** Initializes a texture from a ETC file */
    bool initWithETCFile(const char* file);

    /** sets the min filter, mag filter, wrap s and wrap t texture parameters.
    If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    @code
    when this functon bound to js,the input param are changed
    js: var setTexParameters(var minFilter, var magFilter, var wrapS, var wrapT)
    @endcode
    */ // :3
    void setTexParameters(ccTexParams* texParams); // :3

    /** sets antialias texture parameters:
    - GL_TEXTURE_MIN_FILTER = GL_LINEAR
    - GL_TEXTURE_MAG_FILTER = GL_LINEAR

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    */ // :3
    void setAntiAliasTexParameters(); // :3

    /** sets alias texture parameters:
    - GL_TEXTURE_MIN_FILTER = GL_NEAREST
    - GL_TEXTURE_MAG_FILTER = GL_NEAREST

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    */ // :3
    void setAliasTexParameters(); // :3


    /** Generates mipmap images for the texture.
    It only works if the texture size is POT (power of 2).
    @since v0.99.0
    */ // :3
    void generateMipmap(); // :3

    /** returns the pixel format.
     @since v2.0
     */ // :3
    const char* stringForFormat(); // :3

    /** returns the bits-per-pixel of the in-memory OpenGL texture
    @since v1.0
    */ // :3
    unsigned int bitsPerPixelForFormat(); // :3

    /** Helper functions that returns bits per pixels for a given format.
     @since v2.0
     */ // :3
    unsigned int bitsPerPixelForFormat(CCTexture2DPixelFormat format); // :3

    /** sets the default pixel format for UIImagescontains alpha channel.
    If the UIImage contains alpha channel, then the options are:
    - generate 32-bit textures: kCCTexture2DPixelFormat_RGBA8888 (default one)
    - generate 24-bit textures: kCCTexture2DPixelFormat_RGB888
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGBA4444
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGB5A1
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGB565
    - generate 8-bit textures: kCCTexture2DPixelFormat_A8 (only use it if you use just 1 color)

    How does it work ?
    - If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
    - If the image is an RGB (without Alpha) then: If the default pixel format is RGBA8888 then a RGBA8888 (32-bit) will be used. Otherwise a RGB565 (16-bit texture) will be used.

    This parameter is not valid for PVR / PVR.CCZ images.

    @since v0.8
    */ // :3
    static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format); // :3

    /** returns the alpha pixel format
    @since v0.8
    @js getDefaultAlphaPixelFormat
    */ // :3
    static CCTexture2DPixelFormat defaultAlphaPixelFormat(); // :3

    /** treats (or not) PVR files as if they have alpha premultiplied.
     Since it is impossible to know at runtime if the PVR images have the alpha channel premultiplied, it is
     possible load them as if they have (or not) the alpha channel premultiplied.

     By default it is disabled.

     @since v0.99.5
     */ // :3
    static void PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied); // :3

    /** content size */
    const CCSize& getContentSizeInPixels();

    bool hasPremultipliedAlpha();
    bool hasMipmaps();

	void releaseGLTexture();

private:
    bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);
public:
    // By default PVR images are treated as if they don't have the alpha channel premultiplied
    bool m_bPVRHaveAlphaPremultiplied;

    /** pixel format of the texture */
    CC_PROPERTY_READONLY(CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat)
    /** width in pixels */
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
    /** height in pixels */
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)

    /** texture name */
    CC_PROPERTY_READONLY(GLuint, m_uName, Name)

    /** texture max S */
    CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
    /** texture max T */
    CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
    /** content size */
    CC_PROPERTY_READONLY(CCSize, m_tContentSize, ContentSize)

    /** whether or not the texture has their Alpha premultiplied */
    bool m_bHasPremultipliedAlpha;

    bool m_bHasMipmaps;

    /** shader program used by drawAtPoint and drawInRect */
    CC_PROPERTY(CCGLProgram*, m_pShaderProgram, ShaderProgram);
};

// end of textures group
/// @}

NS_CC_END

#endif //__CCTEXTURE2D_H__

