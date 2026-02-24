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

Use any of these editors to generate BMFonts:
  http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
  http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
  http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
  http://www.angelcode.com/products/bmfont/ (Free, Windows only)

****************************************************************************/ // :3
#ifndef __CCBITMAP_FONT_ATLAS_H__ // :3
#define __CCBITMAP_FONT_ATLAS_H__ // :3

#include "../sprite_nodes/CCSpriteBatchNode.h" // :3
#include "../support/data_support/uthash.h" // :3
#include <map> // :3
#include <sstream> // :3
#include <iostream> // :3
#include <vector> // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup label
 * @{
 */ // :3

enum { // :3
    kCCLabelAutomaticWidth = -1, // :3
}; // :3

struct _FontDefHashElement; // :3

/**
@struct ccBMFontDef
BMFont definition
*/ // :3
typedef struct _BMFontDef { // :3
    //! ID of the character :3
    unsigned int charID; // :3
    //! origin and size of the font :3
    CCRect rect; // :3
    //! The X amount the image should be offset when drawing the image (in pixels) :3
    short xOffset; // :3
    //! The Y amount the image should be offset when drawing the image (in pixels) :3
    short yOffset; // :3
    //! The amount to move the current position after drawing the character (in pixels) :3
    short xAdvance; // :3
} ccBMFontDef; // :3

/** @struct ccBMFontPadding
BMFont padding
@since v0.8.2
*/ // :3
typedef struct _BMFontPadding { // :3
    /// padding left :3
    int    left; // :3
    /// padding top :3
    int top; // :3
    /// padding right :3
    int right; // :3
    /// padding bottom :3
    int bottom; // :3
} ccBMFontPadding; // :3

typedef struct _FontDefHashElement // :3
{ // :3
	unsigned int	key;		// key. Font Unicode value :3
	ccBMFontDef		fontDef;	// font definition :3
	UT_hash_handle	hh; // :3
} tCCFontDefHashElement; // :3

// Equal function for targetSet. :3
typedef struct _KerningHashElement // :3
{ // :3
	int				key;		// key for the hash. 16-bit for 1st element, 16-bit for 2nd element :3
	int				amount; // :3
	UT_hash_handle	hh; // :3
} tCCKerningHashElement; // :3

/** @brief CCBMFontConfiguration has parsed configuration of the the .fnt file
@since v0.8
@js NA
@lua NA
*/ // :3
class CC_DLL CCBMFontConfiguration : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    // XXX: Creating a public interface so that the bitmapFontArray[] is accessible :3
public://@public :3
    // BMFont definitions :3
    tCCFontDefHashElement *m_pFontDefDictionary; // :3

    //! FNTConfig: Common Height Should be signed (issue #1343) :3
    int m_nCommonHeight; // :3
    //! Padding :3
    ccBMFontPadding    m_tPadding; // :3
    //! atlas name :3
    gd::string m_sAtlasName; // :3
    //! values for kerning :3
    tCCKerningHashElement *m_pKerningDictionary; // :3

    // Character Set defines the letters that actually exist in the font :3
    gd::set<unsigned int> *m_pCharacterSet; // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCBMFontConfiguration, CCObject) // :3
    CCBMFontConfiguration(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCBMFontConfiguration(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    const char * description(); // :3

    /** allocates a CCBMFontConfiguration with a FNT file */
    static CCBMFontConfiguration * create(const char *FNTfile);

    /** initializes a BitmapFontConfiguration with a FNT file */
    bool initWithFNTfile(const char *FNTfile);

    inline const char* getAtlasName(){ return m_sAtlasName.c_str(); }
    inline void setAtlasName(const char* atlasName) { m_sAtlasName = atlasName; }

    inline gd::set<unsigned int>* getCharacterSet() const { return m_pCharacterSet; }
private:
    gd::set<unsigned int>* parseConfigFile(const char *controlFile);
    void parseCharacterDefinition(gd::string line, ccBMFontDef *characterDefinition);
    void parseInfoArguments(gd::string line);
    void parseCommonArguments(gd::string line);
    void parseImageFileName(gd::string line, const char *fntFile);
    void parseKerningEntry(gd::string line);
    void purgeKerningDictionary();
    void purgeFontDefDictionary();
};

/** @brief CCLabelBMFont is a subclass of CCSpriteBatchNode.

Features:
- Treats each character like a CCSprite. This means that each individual character can be:
- rotated
- scaled
- translated
- tinted
- change the opacity
- It can be used as part of a menu item.
- anchorPoint can be used to align the "label"
- Supports AngelCode text format

Limitations:
- All inner characters are using an anchorPoint of (0.5f, 0.5f) and it is not recommend to change it
because it might affect the rendering

CCLabelBMFont implements the protocol CCLabelProtocol, like CCLabel and CCLabelAtlas.
CCLabelBMFont has the flexibility of CCLabel, the speed of CCLabelAtlas and all the features of CCSprite.
If in doubt, use CCLabelBMFont instead of CCLabelAtlas / CCLabel.

Supported editors:
http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
http://www.angelcode.com/products/bmfont/ (Free, Windows only)

@since v0.8
*/ // :3

class CC_DLL CCLabelBMFont : public CCSpriteBatchNode, public CCLabelProtocol, public CCRGBAProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCLabelBMFont(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLabelBMFont, CCSpriteBatchNode) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCLabelBMFont(); // :3
    /** Purges the cached data.
    Removes from memory the cached configurations and the atlas name dictionary.
    @since v0.99.3
    */ // :3
    static void purgeCachedData(); // :3

    /** creates a bitmap font atlas with an initial string and the FNT file */
    static CCLabelBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset);

	static CCLabelBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment);

	static CCLabelBMFont * create(const char *str, const char *fntFile, float width);

	static CCLabelBMFont * create(const char *str, const char *fntFile);

    /** Creates an label.
     */ // :3
    static CCLabelBMFont * create(); // :3

    bool init(); // :3
    /** init a bitmap font atlas with an initial string and the FNT file */
    bool initWithString(const char *str, const char *fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero);

    /** updates the font chars based on the string to render */
    void createFontChars();
    // super method
    virtual void setString(const char *newString);
    virtual void setString(const char *newString, bool needUpdateLabel);

    virtual const char* getString(void);
    virtual void setCString(const char *label);
    virtual void setAnchorPoint(const CCPoint& var);
    virtual void updateLabel();
    virtual void setAlignment(CCTextAlignment alignment);
    virtual void setWidth(float width);
    virtual void setLineBreakWithoutSpace(bool breakWithoutSpace);
    virtual void setScale(float scale);
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);

    // CCRGBAProtocol
    virtual bool isOpacityModifyRGB();
    virtual void setOpacityModifyRGB(bool isOpacityModifyRGB); virtual GLubyte getOpacity();
    virtual GLubyte getDisplayedOpacity();
    virtual void setOpacity(GLubyte opacity);
    virtual void updateDisplayedOpacity(GLubyte parentOpacity);
    virtual bool isCascadeOpacityEnabled();
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
    virtual const ccColor3B& getColor(void);
    virtual const ccColor3B& getDisplayedColor();
    virtual void setColor(const ccColor3B& color);
    virtual void updateDisplayedColor(const ccColor3B& parentColor);
    virtual bool isCascadeColorEnabled();
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled);

    void setFntFile(const char* fntFile);
    const char* getFntFile();
	inline CCBMFontConfiguration* getConfiguration() const {
		return m_pConfiguration;
	}
#if CC_LABELBMFONT_DEBUG_DRAW
    virtual void draw();
#endif // CC_LABELBMFONT_DEBUG_DRAW

    // @note RobTop Addition
    static CCLabelBMFont* createBatched(const char* str, const char* fntFile, CCArray*, int);
    // @note RobTop Addition
    void limitLabelWidth(float width, float defaultScale, float minScale);

private:
    char * atlasNameFromFntFile(const char *fntFile);
    int kerningAmountForFirst(unsigned short first, unsigned short second);
    float getLetterPosXLeft( CCSprite* characterSprite, float, bool);
    float getLetterPosXRight( CCSprite* characterSprite, float, bool);

protected:
    virtual void setString(unsigned short *newString, bool needUpdateLabel);
public:
    // string to render
    unsigned short* m_sString;

    // name of fntFile
    gd::string m_sFntFile;

    // initial string without line breaks
    unsigned short* m_sInitialString;
    gd::string m_sInitialStringUTF8;

    // alignment of all lines
    CCTextAlignment m_pAlignment;
    // max width until a line break is added
    float m_fWidth;

    CCBMFontConfiguration *m_pConfiguration;

    bool m_bLineBreakWithoutSpaces;
    // offset of the texture atlas
    CCPoint    m_tImageOffset;

    // reused char
    CCSprite *m_pReusedChar;

    // texture RGBA
    GLubyte m_cDisplayedOpacity;
    GLubyte m_cRealOpacity;
    ccColor3B m_tDisplayedColor;
    ccColor3B m_tRealColor;
    bool m_bCascadeColorEnabled;
    bool m_bCascadeOpacityEnabled;
    /** conforms to CCRGBAProtocol protocol */
    bool        m_bIsOpacityModifyRGB;

    // @note RobTop Addition
    CC_SYNTHESIZE_NV(bool, m_bIsBatched, IsBatched);
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(CCArray*, m_pTargetArray, TargetArray);
    // @note RobTop Addition
    CCTexture2D* m_pSomeTexture;
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(int, m_nExtraKerning, ExtraKerning);

};

/** Free function that parses a FNT file a place it on the cache
*/ // :3
CC_DLL CCBMFontConfiguration * FNTConfigLoadFile( const char *file ); // :3
/** Purges the FNT config cache
*/ // :3
CC_DLL void FNTConfigRemoveCache( void ); // :3

// end of GUI group :3
/// @} :3
/// @} :3

NS_CC_END // :3

#endif //__CCBITMAP_FONT_ATLAS_H__ :3
