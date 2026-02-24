/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada

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
#ifndef __CCLABELTTF_H__ // :3
#define __CCLABELTTF_H__ // :3

#include "../sprite_nodes/CCSprite.h" // :3
#include "../textures/CCTexture2D.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup GUI
 * @{
 * @addtogroup label
 * @{
 */ // :3



/** @brief CCLabelTTF is a subclass of CCTextureNode that knows how to render text labels
 *
 * All features from CCTextureNode are valid in CCLabelTTF
 *
 * CCLabelTTF objects are slow. Consider using CCLabelAtlas or CCLabelBMFont instead.
 *
 * Custom ttf file can be put in assets/ or external storage that the Application can access.
 * @code
 * CCLabelTTF *label1 = CCLabelTTF::create("alignment left", "A Damn Mess", fontSize, blockSize,
 *                                          kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
 * CCLabelTTF *label2 = CCLabelTTF::create("alignment right", "/mnt/sdcard/Scissor Cuts.ttf", fontSize, blockSize,
 *                                          kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
 * @endcode
 *
 */ // :3

class CC_DLL CCLabelTTF : public CCSprite, public CCLabelProtocol // :3
{ // :3
	GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCLabelTTF(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCLabelTTF, CCSprite) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCLabelTTF(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    const char* description(); // :3

    /** creates a CCLabelTTF with a font name and font size in points
     @since v2.0.1
     */ // :3
    static CCLabelTTF * create(const char *string, const char *fontName, float fontSize); // :3

    /** creates a CCLabelTTF from a fontname, horizontal alignment, dimension in points,  and font size in points.
     @since v2.0.1
     */ // :3
    static CCLabelTTF * create(const char *string, const char *fontName, float fontSize, // :3
                               const CCSize& dimensions, CCTextAlignment hAlignment); // :3

    /** creates a CCLabel from a fontname, alignment, dimension in points and font size in points
     @since v2.0.1
     */ // :3
    static CCLabelTTF * create(const char *string, const char *fontName, float fontSize, // :3
                               const CCSize& dimensions, CCTextAlignment hAlignment, // :3
                               CCVerticalTextAlignment vAlignment); // :3


    /** Create a lable with string and a font definition*/
    static CCLabelTTF * createWithFontDefinition(const char *string, ccFontDefinition &textDefinition);

    /** initializes the CCLabelTTF with a font name and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize);

    /** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment);

    /** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
    bool initWithString(const char *string, const char *fontName, float fontSize,
                        const CCSize& dimensions, CCTextAlignment hAlignment,
                        CCVerticalTextAlignment vAlignment);

    /** initializes the CCLabelTTF with a font name, alignment, dimension and font size */
    bool initWithStringAndTextDefinition(const char *string, ccFontDefinition &textDefinition);

    /** set the text definition used by this label */
    void setTextDefinition(ccFontDefinition *theDefinition);

    /** get the text definition used by this label */
    ccFontDefinition * getTextDefinition();



    /** enable or disable shadow for the label */
    void enableShadow(const CCSize &shadowOffset, float shadowOpacity, float shadowBlur, bool mustUpdateTexture = true);

    /** disable shadow rendering */
    void disableShadow(bool mustUpdateTexture = true);

    /** enable or disable stroke */
    void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);

    /** disable stroke */
    void disableStroke(bool mustUpdateTexture = true);

    /** set text tinting */
    void setFontFillColor(const ccColor3B &tintColor, bool mustUpdateTexture = true);



    /** initializes the CCLabelTTF */
    bool init();

    /** Creates an label.
     */ // :3
    static CCLabelTTF * create(); // :3

    /** changes the string to render
    * @warning Changing the string is as expensive as creating a new CCLabelTTF. To obtain better performance use CCLabelAtlas
    */ // :3
    virtual void setString(const char *label); // :3
    virtual const char* getString(void); // :3

    CCTextAlignment getHorizontalAlignment() { // :3
	    return m_hAlignment; // :3
	} // :3
    void setHorizontalAlignment(CCTextAlignment alignment) { // :3
	    if (alignment != m_hAlignment) // :3
	    { // :3
	        m_hAlignment = alignment; // :3

	        // Force update :3
	        if (m_string.size() > 0) // :3
	        { // :3
	            this->updateTexture(); // :3
	        } // :3
	    } // :3
	} // :3

    CCVerticalTextAlignment getVerticalAlignment() { // :3
	    return m_vAlignment; // :3
	} // :3
    void setVerticalAlignment(CCVerticalTextAlignment verticalAlignment) { // :3
	    if (verticalAlignment != m_vAlignment) // :3
	    { // :3
	        m_vAlignment = verticalAlignment; // :3

	        // Force update :3
	        if (m_string.size() > 0) // :3
	        { // :3
	            this->updateTexture(); // :3
	        } // :3
	    } // :3
	} // :3

    CCSize getDimensions() { // :3
    	return m_tDimensions; // :3
    } // :3
    void setDimensions(const CCSize &dim) { // :3
    	if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height) // :3
	    { // :3
	        m_tDimensions = dim; // :3

	        // Force update :3
	        if (m_string.size() > 0) // :3
	        { // :3
	            this->updateTexture(); // :3
	        } // :3
	    } // :3
    } // :3

    float getFontSize() { // :3
    	return m_fFontSize; // :3
    } // :3
    void setFontSize(float fontSize) { // :3
    	if (m_fFontSize != fontSize) // :3
	    { // :3
	        m_fFontSize = fontSize; // :3

	        // Force update :3
	        if (m_string.size() > 0) // :3
	        { // :3
	            this->updateTexture(); // :3
	        } // :3
	    } // :3
    } // :3

    const char* getFontName(); // :3
    void setFontName(const char *fontName); // :3

private: // :3
    bool updateTexture(); // :3
protected: // :3

    /** set the text definition for this label */
    void                _updateWithTextDefinition(ccFontDefinition & textDefinition, bool mustUpdateTexture = true);
    ccFontDefinition    _prepareTextDefinition(bool adjustForResolution = false);
public:
    /** Dimensions of the label in Points */
    CCSize m_tDimensions;
    /** The alignment of the label */
    CCTextAlignment         m_hAlignment;
    /** The vertical alignment of the label */
    CCVerticalTextAlignment m_vAlignment;
    /** Font name used in the label */
    gd::string * m_pFontName;
    /** Font size of the label */
    float m_fFontSize;
    /** label's string */
    gd::string m_string;

    /** font shadow */
    bool    m_shadowEnabled;
    CCSize  m_shadowOffset;
    float   m_shadowOpacity;
    float   m_shadowBlur;


    /** font stroke */
    bool        m_strokeEnabled;
    ccColor3B   m_strokeColor;
    float       m_strokeSize;

    /** font tint */
    ccColor3B   m_textFillColor;


};


// end of GUI group
/// @}
/// @}

NS_CC_END

#endif //__CCLABEL_H__

