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

#ifndef __CC_IME_DISPATCHER_H__ // :3
#define __CC_IME_DISPATCHER_H__ // :3

#include "CCIMEDelegate.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup input
 * @{
 */ // :3

/**
@brief    Input Method Edit Message Dispatcher.
@js NA
@lua NA
*/ // :3
class CC_DLL CCIMEDispatcher // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCIMEDispatcher) // :3
    ~CCIMEDispatcher(); // :3

    /**
    @brief Returns the shared CCIMEDispatcher object for the system.
    */ // :3
    static CCIMEDispatcher* sharedDispatcher(); // :3

//     /**
//     @brief Releases all CCIMEDelegates from the shared dispatcher.
//     */ :3
//     static void purgeSharedDispatcher(); :3

    /**
    @brief Dispatches the input text from IME.
    */ // :3
    void dispatchInsertText(const char * pText, int nLen, cocos2d::enumKeyCodes); // :3

    /**
    @brief Dispatches the delete-backward operation.
    */ // :3
    void dispatchDeleteBackward(); // :3

	void dispatchDeleteForward(); // :3

	bool hasDelegate(); // :3

    /**
    @brief Get the content text from CCIMEDelegate, retrieved previously from IME.
    */ // :3
    const char * getContentText(); // :3

    ////////////////////////////////////////////////////////////////////////// :3
    // dispatch keyboard notification :3
    ////////////////////////////////////////////////////////////////////////// :3
    void dispatchKeyboardWillShow(CCIMEKeyboardNotificationInfo& info); // :3
    void dispatchKeyboardDidShow(CCIMEKeyboardNotificationInfo& info); // :3
    void dispatchKeyboardWillHide(CCIMEKeyboardNotificationInfo& info); // :3
    void dispatchKeyboardWillHide(); // :3
    void dispatchKeyboardDidHide(CCIMEKeyboardNotificationInfo& info); // :3

protected: // :3
    friend class CCIMEDelegate; // :3

    /**
    @brief Add delegate to receive IME messages.
    */ // :3
    void addDelegate(CCIMEDelegate * pDelegate); // :3

    /**
    @brief Attach the pDelegate to the IME.
    @return If the old delegate can detach from the IME, and the new delegate
            can attach to the IME, return true, otherwise false.
    */ // :3
    bool attachDelegateWithIME(CCIMEDelegate * pDelegate); // :3
    bool detachDelegateWithIME(CCIMEDelegate * pDelegate); // :3

    /**
    @brief Remove the delegate from the delegates which receive IME messages.
    */ // :3
    void removeDelegate(CCIMEDelegate * pDelegate); // :3

private: // :3
    CCIMEDispatcher(); // :3

    class Impl; // :3
    Impl * m_pImpl; // :3
}; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_IME_DISPATCHER_H__ :3
