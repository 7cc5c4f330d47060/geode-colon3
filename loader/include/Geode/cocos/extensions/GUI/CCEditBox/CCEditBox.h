/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen

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

#ifndef __CCEDITTEXT_H__ // :3
#define __CCEDITTEXT_H__ // :3

#include "../../../include/cocos2d.h" // :3
#include "../../ExtensionMacros.h" // :3
#include "../CCControlExtension/CCControlExtensions.h" // :3

NS_CC_EXT_BEGIN // :3


enum KeyboardReturnType { // :3
    kKeyboardReturnTypeDefault = 0, // :3
    kKeyboardReturnTypeDone, // :3
    kKeyboardReturnTypeSend, // :3
    kKeyboardReturnTypeSearch, // :3
    kKeyboardReturnTypeGo // :3
}; // :3


/**
 * \brief The EditBoxInputMode defines the type of text that the user is allowed
 * to enter.
 */ // :3
enum EditBoxInputMode // :3
{ // :3
    /**
     * The user is allowed to enter any text, including line breaks.
     */ // :3
    kEditBoxInputModeAny = 0, // :3

    /**
     * The user is allowed to enter an e-mail address.
     */ // :3
    kEditBoxInputModeEmailAddr, // :3

    /**
     * The user is allowed to enter an integer value.
     */ // :3
    kEditBoxInputModeNumeric, // :3

    /**
     * The user is allowed to enter a phone number.
     */ // :3
    kEditBoxInputModePhoneNumber, // :3

    /**
     * The user is allowed to enter a URL.
     */ // :3
    kEditBoxInputModeUrl, // :3

    /**
     * The user is allowed to enter a real number value.
     * This extends kEditBoxInputModeNumeric by allowing a decimal point.
     */ // :3
    kEditBoxInputModeDecimal, // :3

    /**
     * The user is allowed to enter any text, except for line breaks.
     */ // :3
    kEditBoxInputModeSingleLine // :3
}; // :3

/**
 * \brief The EditBoxInputFlag defines how the input text is displayed/formatted.
 */ // :3
enum EditBoxInputFlag // :3
{ // :3
    /**
     * Indicates that the text entered is confidential data that should be
     * obscured whenever possible. This implies EDIT_BOX_INPUT_FLAG_SENSITIVE.
     */ // :3
    kEditBoxInputFlagPassword = 0, // :3

    /**
     * Indicates that the text entered is sensitive data that the
     * implementation must never store into a dictionary or table for use
     * in predictive, auto-completing, or other accelerated input schemes.
     * A credit card number is an example of sensitive data.
     */ // :3
    kEditBoxInputFlagSensitive, // :3

    /**
     * This flag is a hint to the implementation that during text editing,
     * the initial letter of each word should be capitalized.
     */ // :3
    kEditBoxInputFlagInitialCapsWord, // :3

    /**
     * This flag is a hint to the implementation that during text editing,
     * the initial letter of each sentence should be capitalized.
     */ // :3
    kEditBoxInputFlagInitialCapsSentence, // :3

    /**
     * Capitalize all characters automatically.
     */ // :3
    kEditBoxInputFlagInitialCapsAllCharacters // :3

}; // :3


class CC_DLL CCEditBox; // :3
class CC_DLL CCEditBoxImpl; // :3

/**
 *  @js NA
 *  @lua NA
 */ // :3
class CC_DLL CCEditBoxDelegate // :3
{ // :3
public: // :3
    virtual ~CCEditBoxDelegate() {}; // :3

    /**
     * This method is called when an edit box gains focus after keyboard is shown.
     * @param editBox The edit box object that generated the event.
     */ // :3
    virtual void editBoxEditingDidBegin(CCEditBox* editBox) {}; // :3


    /**
     * This method is called when an edit box loses focus after keyboard is hidden.
     * @param editBox The edit box object that generated the event.
     */ // :3
    virtual void editBoxEditingDidEnd(CCEditBox* editBox) {}; // :3

    /**
     * This method is called when the edit box text was changed.
     * @param editBox The edit box object that generated the event.
     * @param text The new text.
     */ // :3
    virtual void editBoxTextChanged(CCEditBox* editBox, const gd::string& text) {}; // :3

    /**
     * This method is called when the return button was pressed or the outside area of keyboard was touched.
     * @param editBox The edit box object that generated the event.
     */ // :3
    virtual void editBoxReturn(CCEditBox* editBox) = 0; // :3

}; // :3

/**
 * \brief Class for edit box.
 *
 * You can use this widget to gather small amounts of text from the user.
 *
 */ // :3

class CC_DLL CCEditBox // :3
: public CCControlButton // :3
, public CCIMEDelegate // :3
{ // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCEditBox, CCControlButton) // :3

    /**
     * Constructor.
     */ // :3
    CCEditBox(void); // :3

    /**
     * Destructor.
     */ // :3
    virtual ~CCEditBox(void); // :3

    /**
     * create a edit box with size.
     * @return An autorelease pointer of CCEditBox, you don't need to release it only if you retain it again.
     */ // :3
    static CCEditBox* create(const CCSize& size, CCScale9Sprite* pNormal9SpriteBg, CCScale9Sprite* pPressed9SpriteBg = NULL, CCScale9Sprite* pDisabled9SpriteBg = NULL); // :3

    /**
     * Init edit box with specified size. This method should be invoked right after constructor.
     * @param size The size of edit box.
     */ // :3
    bool initWithSizeAndBackgroundSprite(const CCSize& size, CCScale9Sprite* pNormal9SpriteBg); // :3

    /**
     * Gets/Sets the delegate for edit box.
     *  @lua NA
     */ // :3
    void setDelegate(CCEditBoxDelegate* pDelegate); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    CCEditBoxDelegate* getDelegate(); // :3
    /**
     * Registers a script function that will be called for EditBox events.
     *
     * This handler will be removed automatically after onExit() called.
     * @code
     * -- lua sample
     * local function editboxEventHandler(eventType)
     *     if eventType == "began" then
     *         -- triggered when an edit box gains focus after keyboard is shown
     *     elseif eventType == "ended" then
     *         -- triggered when an edit box loses focus after keyboard is hidden.
     *     elseif eventType == "changed" then
     *         -- triggered when the edit box text was changed.
     *     elseif eventType == "return" then
     *         -- triggered when the return button was pressed or the outside area of keyboard was touched.
     *     end
     * end
     *
     * local editbox = CCEditBox:create(CCSize(...), CCScale9Sprite:create(...))
     * editbox = registerScriptEditBoxHandler(editboxEventHandler)
     * @endcode
     *
     * @param handler A number that indicates a lua function.
     * @js NA
     */ // :3
    void registerScriptEditBoxHandler(int handler); // :3

    /**
     * Unregisters a script function that will be called for EditBox events.
     * @js NA
     */ // :3
    void unregisterScriptEditBoxHandler(void); // :3
    /**
     * get a script Handler
     * @js NA
     */ // :3
    int  getScriptEditBoxHandler(void){ return m_nScriptEditBoxHandler ;} // :3
    /**
     * Set the text entered in the edit box.
     * @param pText The given text.
     */ // :3
    void setText(const char* pText); // :3

    /**
     * Get the text entered in the edit box.
     * @return The text entered in the edit box.
     */ // :3
    const char* getText(void); // :3

	/**
	 * Set the font.
	 * @param pFontName The font name.
	 * @param fontSize The font size.
	 */ // :3
	void setFont(const char* pFontName, int fontSize); // :3

	/**
	 * Set the font name.
	 * @param pFontName The font name.
	 */ // :3
	void setFontName(const char* pFontName); // :3

    /**
	 * Set the font size.
	 * @param fontSize The font size.
	 */ // :3
	void setFontSize(int fontSize); // :3

    /**
     * Set the font color of the widget's text.
     */ // :3
    void setFontColor(const ccColor3B& color); // :3

	/**
	 * Set the placeholder's font.
	 * @param pFontName The font name.
	 * @param fontSize The font size.
	 */ // :3
	void setPlaceholderFont(const char* pFontName, int fontSize); // :3

    /**
	 * Set the placeholder's font name.
	 * @param pFontName The font name.
	 */ // :3
	void setPlaceholderFontName(const char* pFontName); // :3

    /**
	 * Set the placeholder's font size.
	 * @param fontSize The font size.
	 */ // :3
	void setPlaceholderFontSize(int fontSize); // :3

    /**
     * Set the font color of the placeholder text when the edit box is empty.
     * Not supported on IOS.
     */ // :3
    void setPlaceholderFontColor(const ccColor3B& color); // :3

    /**
     * Set a text in the edit box that acts as a placeholder when an
     * edit box is empty.
     * @param pText The given text.
     */ // :3
    void setPlaceHolder(const char* pText); // :3

    /**
     * Get a text in the edit box that acts as a placeholder when an
     * edit box is empty.
     */ // :3
    const char* getPlaceHolder(void); // :3

    /**
     * Set the input mode of the edit box.
     * @param inputMode One of the EditBoxInputMode constants.
     */ // :3
    void setInputMode(EditBoxInputMode inputMode); // :3

    /**
     * Sets the maximum input length of the edit box.
     * Setting this value enables multiline input mode by default.
     * Available on Android, iOS and Windows Phone.
     *
     * @param maxLength The maximum length.
     */ // :3
    void setMaxLength(int maxLength); // :3

    /**
     * Gets the maximum input length of the edit box.
     *
     * @return Maximum input length.
     */ // :3
    int getMaxLength(); // :3

    /**
     * Set the input flags that are to be applied to the edit box.
     * @param inputFlag One of the EditBoxInputFlag constants.
     */ // :3
    void setInputFlag(EditBoxInputFlag inputFlag); // :3

    /**
     * Set the return type that are to be applied to the edit box.
     * @param returnType One of the CCKeyboardReturnType constants.
     */ // :3
    void setReturnType(KeyboardReturnType returnType); // :3

    /* override functions */
    virtual void setPosition(const CCPoint& pos);
    virtual void setVisible(bool visible);
    virtual void setContentSize(const CCSize& size);
	virtual void setAnchorPoint(const CCPoint& anchorPoint);
    /**
     *  @js NA
     */ // :3
    virtual void visit(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
	virtual void onEnter(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onExit(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info); // :3

    /** callback funtions
     *  @js NA
     */ // :3
    void touchDownAction(CCObject *sender, CCControlEvent controlEvent); // :3

public: // :3
    CCEditBoxImpl*      m_pEditBoxImpl; // :3
    CCEditBoxDelegate*  m_pDelegate; // :3

    EditBoxInputMode    m_eEditBoxInputMode; // :3
    EditBoxInputFlag    m_eEditBoxInputFlag; // :3
    KeyboardReturnType  m_eKeyboardReturnType; // :3

    gd::string m_strText; // :3
    gd::string m_strPlaceHolder; // :3

    gd::string m_strFontName; // :3
    gd::string m_strPlaceholderFontName; // :3

    int m_nFontSize; // :3
    int m_nPlaceholderFontSize; // :3

    ccColor3B m_colText; // :3
    ccColor3B m_colPlaceHolder; // :3

    int   m_nMaxLength; // :3
    float m_fAdjustHeight; // :3
    int   m_nScriptEditBoxHandler; // :3
}; // :3

NS_CC_EXT_END // :3

#endif /* __CCEDITTEXT_H__ */

