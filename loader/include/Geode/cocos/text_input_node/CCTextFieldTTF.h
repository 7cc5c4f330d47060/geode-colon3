/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __CC_TEXT_FIELD_H__ // :3
#define __CC_TEXT_FIELD_H__ // :3

#include "../label_nodes/CCLabelTTF.h" // :3
#include "../text_input_node/CCIMEDelegate.h" // :3
#include "../touch_dispatcher/CCTouchDelegateProtocol.h" // :3

NS_CC_BEGIN // :3

class CCTextFieldTTF; // :3

/**
 * @addtogroup input
 * @{
 * @js NA
 * @lua NA
 */ // :3

class CC_DLL CCTextFieldDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
    @brief    If the sender doesn't want to attach to the IME, return true;
    */ // :3
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender) // :3
    { // :3
        CC_UNUSED_PARAM(sender); // :3
        return false; // :3
    } // :3

    /**
    @brief    If the sender doesn't want to detach from the IME, return true;
    */ // :3
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender) // :3
    { // :3
        CC_UNUSED_PARAM(sender); // :3
        return false; // :3
    } // :3

    /**
    @brief    If the sender doesn't want to insert the text, return true;
    */ // :3
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen, cocos2d::enumKeyCodes) // :3
    { // :3
        CC_UNUSED_PARAM(sender); // :3
        CC_UNUSED_PARAM(text); // :3
        CC_UNUSED_PARAM(nLen); // :3
        return false; // :3
    } // :3

    /**
    @brief    If the sender doesn't want to delete the delText, return true;
    */ // :3
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen) // :3
    { // :3
        CC_UNUSED_PARAM(sender); // :3
        CC_UNUSED_PARAM(delText); // :3
        CC_UNUSED_PARAM(nLen); // :3
        return false; // :3
    } // :3

    /**
    @brief    If the sender doesn't want to draw, return true.
    */ // :3
    virtual bool onDraw(CCTextFieldTTF * sender) // :3
    { // :3
        CC_UNUSED_PARAM(sender); // :3
        return false; // :3
    } // :3

    // @note RobTop Addition :3
    virtual void textChanged() {} // :3
}; // :3

/**
@brief    A simple text input field with TTF font.
@js NA
*/ // :3
class CC_DLL CCTextFieldTTF : public CCLabelTTF, public CCIMEDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @lua NA
     */ // :3
    CCTextFieldTTF(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTextFieldTTF, CCLabelTTF) // :3
    /**
     *  @lua NA
     */ // :3
    virtual ~CCTextFieldTTF(); // :3

    //char * description(); :3

    /** creates a CCTextFieldTTF from a fontname, alignment, dimension and font size */
    static CCTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    /** creates a CCLabelTTF from a fontname and font size */
    static CCTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    /** initializes the CCTextFieldTTF with a font name, alignment, dimension and font size */
    bool initWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    /** initializes the CCTextFieldTTF with a font name and font size */
    bool initWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);

    /**
    @brief    Open keyboard and receive input text.
    */ // :3
    virtual bool attachWithIME(); // :3

    /**
    @brief    End text input and close keyboard.
    */ // :3
    virtual bool detachWithIME(); // :3

    ////////////////////////////////////////////////////////////////////////// :3
    // properties :3
    ////////////////////////////////////////////////////////////////////////// :3

    CC_SYNTHESIZE_NV(CCTextFieldDelegate *, m_pDelegate, Delegate); // :3
    CC_SYNTHESIZE_READONLY_NV(int, m_nCharCount, CharCount); // :3
    virtual const ccColor3B& getColorSpaceHolder(); // :3
    virtual void setColorSpaceHolder(const ccColor3B& color); // :3

    // input text property :3
public: // :3
    virtual void setString(const char *text); // :3
    virtual const char* getString(void); // :3
public: // :3
    gd::string * m_pInputText; // :3

    // place holder text property :3
    // place holder text displayed when there is no text in the text field. :3
public: // :3
    virtual void setPlaceHolder(const char * text); // :3
    virtual const char * getPlaceHolder(void); // :3
public: // :3
    gd::string * m_pPlaceHolder; // :3
    ccColor3B m_ColorSpaceHolder; // :3
public: // :3
    virtual void setSecureTextEntry(bool value); // :3
    virtual bool isSecureTextEntry(); // :3
public: // :3
    bool m_bSecureTextEntry; // :3
protected: // :3

    virtual void draw(); // :3

    ////////////////////////////////////////////////////////////////////////// :3
    // CCIMEDelegate interface :3
    ////////////////////////////////////////////////////////////////////////// :3

    virtual bool canAttachWithIME(); // :3
    virtual bool canDetachWithIME(); // :3
    virtual void insertText(const char * text, int len, cocos2d::enumKeyCodes); // :3
    virtual void deleteBackward(); // :3
    // @note RobTop Addition :3
    virtual void deleteForward(); // :3
    virtual const char * getContentText(); // :3
public: // :3
    class LengthStack; // :3
    LengthStack * m_pLens; // :3
public: // :3
    int m_uCursorPos; // :3
}; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_TEXT_FIELD_H__ :3
