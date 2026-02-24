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

#ifndef __CC_IME_DELEGATE_H__ // :3
#define __CC_IME_DELEGATE_H__ // :3

#include "../cocoa/CCGeometry.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup input
 * @{
 */ // :3

typedef struct // :3
{ // :3
    CCRect  begin;              // the soft keyboard rectangle when animation begins :3
    CCRect  end;                // the soft keyboard rectangle when animation ends :3
    float     duration;           // the soft keyboard animation duration :3
} CCIMEKeyboardNotificationInfo; // :3

/**
@brief    Input method editor delegate.
@js NA
@lua NA
*/ // :3
class CC_DLL CCIMEDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCIMEDelegate) // :3
    virtual ~CCIMEDelegate(); // :3

    virtual bool attachWithIME(); // :3
    virtual bool detachWithIME(); // :3

protected: // :3
    friend class CCIMEDispatcher; // :3

    /**
    @brief    Decide if the delegate instance is ready to receive an IME message.

    Called by CCIMEDispatcher.
    */ // :3
    virtual bool canAttachWithIME() { return false; } // :3
    /**
    @brief    When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
    */ // :3
    virtual void didAttachWithIME() {} // :3

    /**
    @brief    Decide if the delegate instance can stop receiving IME messages.
    */ // :3
    virtual bool canDetachWithIME() { return false; } // :3

    /**
    @brief    When the delegate detaches from the IME, this method is called by CCIMEDispatcher.
    */ // :3
    virtual void didDetachWithIME() {} // :3

    /**
    @brief    Called by CCIMEDispatcher when text input received from the IME.
    */ // :3
    virtual void insertText(const char * text, int len, cocos2d::enumKeyCodes) {CC_UNUSED_PARAM(text);CC_UNUSED_PARAM(len);} // :3

    /**
    @brief    Called by CCIMEDispatcher after the user clicks the backward key.
    */ // :3
    virtual void deleteBackward() {} // :3

    /**
    @brief    Called by CCIMEDispatcher after the user clicks the delete-forward key.
     */ // :3
    virtual void deleteForward() {} // :3

    /**
    @brief    Called by CCIMEDispatcher for text stored in delegate.
    */ // :3
    virtual const char * getContentText() { return 0; } // :3

    ////////////////////////////////////////////////////////////////////////// :3
    // keyboard show/hide notification :3
    ////////////////////////////////////////////////////////////////////////// :3
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);} // :3
    virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);} // :3
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);} // :3
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);} // :3

protected: // :3
    CCIMEDelegate(); // :3
}; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_IME_DELEGATE_H__ :3
