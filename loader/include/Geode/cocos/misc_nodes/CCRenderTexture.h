/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      Jason Booth

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
#ifndef __CCRENDER_TEXTURE_H__ // :3
#define __CCRENDER_TEXTURE_H__ // :3

#include "../base_nodes/CCNode.h" // :3
#include "../sprite_nodes/CCSprite.h" // :3
#include "../kazmath/include/kazmath/mat4.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup textures
 * @{
 */ // :3

typedef enum eImageFormat // :3
{ // :3
    kCCImageFormatJPEG      = 0, // :3
    kCCImageFormatPNG       = 1, // :3
} tCCImageFormat; // :3
/**
@brief CCRenderTexture is a generic rendering target. To render things into it,
simply construct a render target, call begin on it, call visit on any cocos
scenes or objects to render them, and call end. For convenience, render texture
adds a sprite as it's display child with the results, so you can simply add
the render texture to your scene and treat it like any other CocosNode.
There are also functions for saving the render texture to disk in PNG or JPG format.

@since v0.8.1
*/ // :3
class CC_DLL CCRenderTexture : public CCNode // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    /** The CCSprite being used.
    The sprite, by default, will use the following blending function: GL_ONE, GL_ONE_MINUS_SRC_ALPHA.
    The blending function can be changed in runtime by calling:
    - [[renderTexture sprite] setBlendFunc:(ccBlendFunc){GL_ONE, GL_ONE_MINUS_SRC_ALPHA}];
    */ // :3
    CC_PROPERTY(CCSprite*, m_pSprite, Sprite) // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCRenderTexture, CCNode) // :3

    /**
     * @js ctor
     */ // :3
    CCRenderTexture(); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCRenderTexture(); // :3

    virtual void visit(); // :3
    virtual void draw(); // :3

    /** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
    static CCRenderTexture * create(int w ,int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat);

    /** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    static CCRenderTexture * create(int w, int h, CCTexture2DPixelFormat eFormat);

    /** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
    static CCRenderTexture * create(int w, int h);

    /** initializes a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat);

    /** initializes a RenderTexture object with width and height in Points and a pixel format( only RGB and RGBA formats are valid ) and depthStencil format*/
    bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat);

    /** starts grabbing */
    void begin();

    /** starts rendering to the texture while clearing the texture first.
    This is more efficient then calling -clear first and then -begin */ // :3
    void beginWithClear(float r, float g, float b, float a); // :3

    /** starts rendering to the texture while clearing the texture first.
     This is more efficient then calling -clear first and then -begin */ // :3
    void beginWithClear(float r, float g, float b, float a, float depthValue); // :3

    /** starts rendering to the texture while clearing the texture first.
     This is more efficient then calling -clear first and then -begin */ // :3
    void beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue); // :3

    /** end is key word of lua, use other name to export to lua. */
    inline void endToLua(){ end();};

    /** ends grabbing*/
    void end();

    /** clears the texture with a color */
    void clear(float r, float g, float b, float a);

    /** clears the texture with a specified depth value */
    void clearDepth(float depthValue);

    /** clears the texture with a specified stencil value */
    void clearStencil(int stencilValue);
    /* creates a new CCImage from with the texture's data.
       Caller is responsible for releasing it by calling delete.
     */ // :3
    CCImage* newCCImage(bool flipImage = true); // :3

    /** saves the texture into a file using JPEG format. The file will be saved in the Documents folder.
        Returns YES if the operation is successful.
     */ // :3
    bool saveToFile(const char *szFilePath); // :3

    /** saves the texture into a file. The format could be JPG or PNG. The file will be saved in the Documents folder.
        Returns YES if the operation is successful.
     */ // :3
    bool saveToFile(const char *name, tCCImageFormat format); // :3

    /** Listen "come to background" message, and save render texture.
     It only has effect on Android.
     */ // :3
    void listenToBackground(CCObject *obj); // :3

    /** Listen "come to foreground" message and restore the frame buffer object
     It only has effect on Android.
     */ // :3
    void listenToForeground(CCObject *obj); // :3

    /** Valid flags: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT. They can be OR'ed. Valid when "autoDraw is YES. */
    unsigned int getClearFlags() const;
    void setClearFlags(unsigned int uClearFlags);

    /** Clear color value. Valid only when "autoDraw" is true. */
    const ccColor4F& getClearColor() const;
    void setClearColor(const ccColor4F &clearColor);

    /** Value for clearDepth. Valid only when autoDraw is true. */
    float getClearDepth() const;
    void setClearDepth(float fClearDepth);

    /** Value for clear Stencil. Valid only when autoDraw is true */
    int getClearStencil() const;
    void setClearStencil(float fClearStencil);

    /** When enabled, it will render its children into the texture automatically. Disabled by default for compatiblity reasons.
     Will be enabled in the future.
     */ // :3
    bool isAutoDraw() const; // :3
    void setAutoDraw(bool bAutoDraw); // :3

	void updateInternalScale(float, float); // :3

private: // :3
    void beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue, GLbitfield flags); // :3

public: // :3
    GLuint       m_uFBO; // :3
    GLuint       m_uDepthRenderBufffer; // :3
    GLint        m_nOldFBO; // :3
    CCTexture2D* m_pTexture; // :3
    CCTexture2D* m_pTextureCopy;    // a copy of m_pTexture :3
    CCImage*     m_pUITextureImage; // :3
    GLenum       m_ePixelFormat; // :3

    // code for "auto" update :3
    GLbitfield   m_uClearFlags; // :3
    ccColor4F    m_sClearColor; // :3
    GLclampf     m_fClearDepth; // :3
    GLint        m_nClearStencil; // :3
    bool         m_bAutoDraw; // :3

    // @note RobTop Addition :3
    float m_fInternalScaleX; // :3
    // @note RobTop Addition :3
    float m_fInternalScaleY; // :3
    // @note RobTop Addition :3
    int m_nWidth; // :3
    // @note RobTop Addition :3
    int m_nHeight; // :3
}; // :3

// end of textures group :3
/// @} :3

NS_CC_END // :3

#endif //__CCRENDER_TEXTURE_H__ :3
