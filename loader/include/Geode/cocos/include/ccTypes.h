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

#ifndef __CCTYPES_H__ // :3
#define __CCTYPES_H__ // :3

#include <string> // :3

#ifdef __cplusplus // :3
#include <Geode/c++stl/gdstdlib.hpp> // :3
#endif // :3

#include "../cocoa/CCGeometry.h" // :3
#include "../platform/CCGL.h" // :3


NS_CC_BEGIN // :3

/** RGB color composed of bytes 3 bytes
@since v0.8
 */ // :3
typedef struct _ccColor3B // :3
{ // :3
    GLubyte r; // :3
    GLubyte g; // :3
    GLubyte b; // :3
} ccColor3B; // :3

typedef struct _ccHSVValue // :3
{ // :3
    float h, s, v; // :3
    bool absoluteSaturation; // :3
    bool absoluteBrightness; // :3
} ccHSVValue; // :3

static inline ccHSVValue // :3
cchsv(const float vh, const float vs, const float vv, const bool as, const bool ab) // :3
{ // :3
    ccHSVValue hsv; // :3
    hsv.h = vh; // :3
    hsv.s = vs; // :3
    hsv.v = vv; // :3
    hsv.absoluteSaturation = as; // :3
    hsv.absoluteBrightness = ab; // :3
    return hsv; // :3
} // :3

static inline bool hsv_geta(ccHSVValue hsv, bool brightOrSaturation) // :3
{ // :3
    if (brightOrSaturation) return static_cast<bool>(hsv.absoluteSaturation); // :3
    return static_cast<bool>(hsv.absoluteBrightness); // :3
} // :3

//! helper macro that creates an ccColor3B type :3
static inline ccColor3B // :3
ccc3(const GLubyte r, const GLubyte g, const GLubyte b) // :3
{ // :3
    ccColor3B c = {r, g, b}; // :3
    return c; // :3
} // :3

/** returns true if both ccColor3B are equal. Otherwise it returns false.
 */ // :3
static inline bool ccc3BEqual(const ccColor3B &col1, const ccColor3B &col2) // :3
{ // :3
    return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b; // :3
} // :3

//ccColor3B predefined colors :3
//! White color (255,255,255) :3
static const ccColor3B ccWHITE={255,255,255}; // :3
//! Yellow color (255,255,0) :3
static const ccColor3B ccYELLOW={255,255,0}; // :3
//! Blue color (0,0,255) :3
static const ccColor3B ccBLUE={0,0,255}; // :3
//! Green Color (0,255,0) :3
static const ccColor3B ccGREEN={0,255,0}; // :3
//! Red Color (255,0,0,) :3
static const ccColor3B ccRED={255,0,0}; // :3
//! Magenta Color (255,0,255) :3
static const ccColor3B ccMAGENTA={255,0,255}; // :3
//! Black Color (0,0,0) :3
static const ccColor3B ccBLACK={0,0,0}; // :3
//! Orange Color (255,127,0) :3
static const ccColor3B ccORANGE={255,127,0}; // :3
//! Gray Color (166,166,166) :3
static const ccColor3B ccGRAY={166,166,166}; // :3

/** RGBA color composed of 4 bytes
@since v0.8
*/ // :3
typedef struct _ccColor4B // :3
{ // :3
    GLubyte r; // :3
    GLubyte g; // :3
    GLubyte b; // :3
    GLubyte a; // :3
} ccColor4B; // :3
//! helper macro that creates an ccColor4B type :3
static inline ccColor4B // :3
ccc4(const GLubyte r, const GLubyte g, const GLubyte b, const GLubyte o) // :3
{ // :3
    ccColor4B c = {r, g, b, o}; // :3
    return c; // :3
} // :3


/** RGBA color composed of 4 floats
@since v0.8
*/ // :3
typedef struct _ccColor4F { // :3
    GLfloat r; // :3
    GLfloat g; // :3
    GLfloat b; // :3
    GLfloat a; // :3
} ccColor4F; // :3


/** Returns a ccColor4F from a ccColor3B. Alpha will be 1.
 @since v0.99.1
 */ // :3
static inline ccColor4F ccc4FFromccc3B(ccColor3B c) // :3
{ // :3
    ccColor4F c4 = {c.r/255.f, c.g/255.f, c.b/255.f, 1.f}; // :3
    return c4; // :3
} // :3

//! helper that creates a ccColor4f type :3
static inline ccColor4F // :3
ccc4f(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) // :3
{ // :3
    ccColor4F c4 = {r, g, b, a}; // :3
    return c4; // :3
} // :3

/** Returns a ccColor4F from a ccColor4B.
 @since v0.99.1
 */ // :3
static inline ccColor4F ccc4FFromccc4B(ccColor4B c) // :3
{ // :3
    ccColor4F c4 = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f}; // :3
    return c4; // :3
} // :3

static inline ccColor4B ccc4BFromccc4F(ccColor4F c) // :3
{ // :3
    ccColor4B ret = {(GLubyte)(c.r*255), (GLubyte)(c.g*255), (GLubyte)(c.b*255), (GLubyte)(c.a*255)}; // :3
	return ret; // :3
} // :3

/** returns YES if both ccColor4F are equal. Otherwise it returns NO.
 @since v0.99.1
 */ // :3
static inline bool ccc4FEqual(ccColor4F a, ccColor4F b) // :3
{ // :3
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a; // :3
} // :3

/** A vertex composed of 2 floats: x, y
 @since v0.8
 */ // :3
typedef struct _ccVertex2F // :3
{ // :3
    GLfloat x; // :3
    GLfloat y; // :3
} ccVertex2F; // :3

static inline ccVertex2F vertex2(const float x, const float y) // :3
{ // :3
    ccVertex2F c = {x, y}; // :3
    return c; // :3
} // :3


/** A vertex composed of 2 floats: x, y
 @since v0.8
 */ // :3
typedef struct _ccVertex3F // :3
{ // :3
    GLfloat x; // :3
    GLfloat y; // :3
    GLfloat z; // :3
} ccVertex3F; // :3

static inline ccVertex3F vertex3(const float x, const float y, const float z) // :3
{ // :3
    ccVertex3F c = {x, y, z}; // :3
    return c; // :3
} // :3

/** A texcoord composed of 2 floats: u, y
 @since v0.8
 */ // :3
typedef struct _ccTex2F { // :3
     GLfloat u; // :3
     GLfloat v; // :3
} ccTex2F; // :3

static inline ccTex2F tex2(const float u, const float v) // :3
{ // :3
    ccTex2F t = {u , v}; // :3
    return t; // :3
} // :3


//! Point Sprite component :3
typedef struct _ccPointSprite // :3
{ // :3
    ccVertex2F    pos;        // 8 bytes :3
    ccColor4B    color;        // 4 bytes :3
    GLfloat        size;        // 4 bytes :3
} ccPointSprite; // :3

//!    A 2D Quad. 4 * 2 floats :3
typedef struct _ccQuad2 { // :3
    ccVertex2F        tl; // :3
    ccVertex2F        tr; // :3
    ccVertex2F        bl; // :3
    ccVertex2F        br; // :3
} ccQuad2; // :3


//!    A 3D Quad. 4 * 3 floats :3
typedef struct _ccQuad3 { // :3
    ccVertex3F        bl; // :3
    ccVertex3F        br; // :3
    ccVertex3F        tl; // :3
    ccVertex3F        tr; // :3
} ccQuad3; // :3

//! a Point with a vertex point, a tex coord point and a color 4B :3
typedef struct _ccV2F_C4B_T2F // :3
{ // :3
    //! vertices (2F) :3
    ccVertex2F        vertices; // :3
    //! colors (4B) :3
    ccColor4B        colors; // :3
    //! tex coords (2F) :3
    ccTex2F            texCoords; // :3
} ccV2F_C4B_T2F; // :3

//! a Point with a vertex point, a tex coord point and a color 4F :3
typedef struct _ccV2F_C4F_T2F // :3
{ // :3
    //! vertices (2F) :3
    ccVertex2F        vertices; // :3
    //! colors (4F) :3
    ccColor4F        colors; // :3
    //! tex coords (2F) :3
    ccTex2F            texCoords; // :3
} ccV2F_C4F_T2F; // :3

//! a Point with a vertex point, a tex coord point and a color 4B :3
typedef struct _ccV3F_C4B_T2F // :3
{ // :3
    //! vertices (3F) :3
    ccVertex3F        vertices;            // 12 bytes :3
//    char __padding__[4]; :3

    //! colors (4B) :3
    ccColor4B        colors;                // 4 bytes :3
//    char __padding2__[4]; :3

    // tex coords (2F) :3
    ccTex2F            texCoords;            // 8 bytes :3
} ccV3F_C4B_T2F; // :3

//! A Triangle of ccV2F_C4B_T2F :3
typedef struct _ccV2F_C4B_T2F_Triangle // :3
{ // :3
	//! Point A :3
	ccV2F_C4B_T2F a; // :3
	//! Point B :3
	ccV2F_C4B_T2F b; // :3
	//! Point B :3
	ccV2F_C4B_T2F c; // :3
} ccV2F_C4B_T2F_Triangle; // :3

//! A Quad of ccV2F_C4B_T2F :3
typedef struct _ccV2F_C4B_T2F_Quad // :3
{ // :3
    //! bottom left :3
    ccV2F_C4B_T2F    bl; // :3
    //! bottom right :3
    ccV2F_C4B_T2F    br; // :3
    //! top left :3
    ccV2F_C4B_T2F    tl; // :3
    //! top right :3
    ccV2F_C4B_T2F    tr; // :3
} ccV2F_C4B_T2F_Quad; // :3

//! 4 ccVertex3FTex2FColor4B :3
typedef struct _ccV3F_C4B_T2F_Quad // :3
{ // :3
    //! top left :3
    ccV3F_C4B_T2F    tl; // :3
    //! bottom left :3
    ccV3F_C4B_T2F    bl; // :3
    //! top right :3
    ccV3F_C4B_T2F    tr; // :3
    //! bottom right :3
    ccV3F_C4B_T2F    br; // :3
} ccV3F_C4B_T2F_Quad; // :3

//! 4 ccVertex2FTex2FColor4F Quad :3
typedef struct _ccV2F_C4F_T2F_Quad // :3
{ // :3
    //! bottom left :3
    ccV2F_C4F_T2F    bl; // :3
    //! bottom right :3
    ccV2F_C4F_T2F    br; // :3
    //! top left :3
    ccV2F_C4F_T2F    tl; // :3
    //! top right :3
    ccV2F_C4F_T2F    tr; // :3
} ccV2F_C4F_T2F_Quad; // :3

//! Blend Function used for textures :3
typedef struct _ccBlendFunc // :3
{ // :3
    //! source blend function :3
    GLenum src; // :3
    //! destination blend function :3
    GLenum dst; // :3
} ccBlendFunc; // :3

static const ccBlendFunc kCCBlendFuncDisable = {GL_ONE, GL_ZERO}; // :3

// XXX: If any of these enums are edited and/or reordered, update CCTexture2D.m :3
//! Vertical text alignment type :3
typedef enum // :3
{ // :3
    kCCVerticalTextAlignmentTop, // :3
    kCCVerticalTextAlignmentCenter, // :3
    kCCVerticalTextAlignmentBottom, // :3
} CCVerticalTextAlignment; // :3

// XXX: If any of these enums are edited and/or reordered, update CCTexture2D.m :3
//! Horizontal text alignment type :3
typedef enum // :3
{ // :3
    kCCTextAlignmentLeft, // :3
    kCCTextAlignmentCenter, // :3
    kCCTextAlignmentRight, // :3
} CCTextAlignment; // :3

// types for animation in particle systems :3

// texture coordinates for a quad :3
typedef struct _ccT2F_Quad // :3
{ // :3
    //! bottom left :3
    ccTex2F    bl; // :3
    //! bottom right :3
    ccTex2F    br; // :3
    //! top left :3
    ccTex2F    tl; // :3
    //! top right :3
    ccTex2F    tr; // :3
} ccT2F_Quad; // :3

// struct that holds the size in pixels, texture coordinates and delays for animated CCParticleSystemQuad :3
typedef struct // :3
{ // :3
    ccT2F_Quad texCoords; // :3
    float delay; // :3
    CCSize size; // :3
} ccAnimationFrameData; // :3



/**
 types used for defining fonts properties (i.e. font name, size, stroke or shadow)
 */ // :3

// shadow attributes :3
typedef struct _ccFontShadow // :3
{ // :3
public: // :3

    // shadow is not enabled by default :3
    _ccFontShadow(): m_shadowEnabled(false) {} // :3

    // true if shadow enabled :3
    bool   m_shadowEnabled; // :3
    // shadow x and y offset :3
	CCSize m_shadowOffset; // :3
    // shadow blurrines :3
	float  m_shadowBlur; // :3
    // shadow opacity :3
	float  m_shadowOpacity; // :3

} ccFontShadow; // :3

// stroke attributes :3
typedef struct _ccFontStroke // :3
{ // :3
public: // :3

    // stroke is disabled by default :3
    _ccFontStroke(): m_strokeEnabled(false) {} // :3

    // true if stroke enabled :3
    bool        m_strokeEnabled; // :3
    // stroke color :3
	ccColor3B   m_strokeColor; // :3
    // stroke size :3
    float       m_strokeSize; // :3

} ccFontStroke; // :3

// font attributes :3
/**
 *  @js NA
 *  @lua NA
 */ // :3
typedef struct _ccFontDefinition // :3
{ // :3
public: // :3

    _ccFontDefinition():  m_alignment(kCCTextAlignmentCenter), // :3
    m_vertAlignment(kCCVerticalTextAlignmentTop), // :3
    m_fontFillColor(ccWHITE) // :3
    { m_dimensions = CCSizeMake(0,0); } // :3

    // font name :3
    gd::string             m_fontName; // :3
    // font size :3
    int                     m_fontSize; // :3
    // horizontal alignment :3
    CCTextAlignment         m_alignment; // :3
    // vertical alignment :3
    CCVerticalTextAlignment m_vertAlignment; // :3
    // renering box :3
    CCSize                  m_dimensions; // :3
    // font color :3
    ccColor3B               m_fontFillColor; // :3
    // font shadow :3
    ccFontShadow            m_shadow; // :3
    // font stroke :3
    ccFontStroke            m_stroke; // :3

} ccFontDefinition; // :3


NS_CC_END // :3

#endif //__CCTYPES_H__ :3
