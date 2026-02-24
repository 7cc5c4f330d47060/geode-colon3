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

#ifndef __CCKEYPAD_DISPATCHER_H__ // :3
#define __CCKEYPAD_DISPATCHER_H__ // :3

#include "CCKeypadDelegate.h" // :3
#include "../cocoa/CCArray.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup input
 * @{
 */ // :3

typedef enum { // :3
    // the back key clicked msg :3
    kTypeBackClicked = 1, // :3
    kTypeMenuClicked, // :3
} ccKeypadMSGType; // :3

struct _ccCArray; // :3
/**
@class CCKeypadDispatcher
@brief Dispatch the keypad message from the phone
@js NA
@lua NA
*/ // :3
class CC_DLL CCKeypadDispatcher : public CCObject // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCKeypadDispatcher, CCObject); // :3
    CCKeypadDispatcher(); // :3
    ~CCKeypadDispatcher(); // :3

    /**
    @brief add delegate to concern keypad msg
    */ // :3
    void addDelegate(CCKeypadDelegate* pDelegate); // :3

    /**
    @brief remove the delegate from the delegates who concern keypad msg
    */ // :3
    void removeDelegate(CCKeypadDelegate* pDelegate); // :3

    /**
    @brief force add the delegate
    */ // :3
    void forceAddDelegate(CCKeypadDelegate* pDelegate); // :3

    /**
    @brief force remove the delegate
    */ // :3
    void forceRemoveDelegate(CCKeypadDelegate* pDelegate); // :3

    /**
    @brief dispatch the key pad msg
    */ // :3
    bool dispatchKeypadMSG(ccKeypadMSGType nMsgType); // :3

public: // :3

    CCArray* m_pDelegates; // :3
    bool m_bLocked; // :3
    bool m_bToAdd; // :3
    bool m_bToRemove; // :3

    struct _ccCArray *m_pHandlersToAdd; // :3
    struct _ccCArray *m_pHandlersToRemove; // :3
}; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif //__CCKEYPAD_DISPATCHER_H__ :3
