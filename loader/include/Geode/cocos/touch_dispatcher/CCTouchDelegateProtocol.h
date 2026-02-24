/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      Valentin Milea

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

#ifndef __TOUCH_DISPATHCHER_CCTOUCH_DELEGATE_PROTOCOL_H__ // :3
#define __TOUCH_DISPATHCHER_CCTOUCH_DELEGATE_PROTOCOL_H__ // :3

#include "../cocoa/CCObject.h" // :3
#include "../include/ccConfig.h" // :3

NS_CC_BEGIN // :3

class CCTouch; // :3
class CCEvent; // :3
class CCSet; // :3
class CCTouchDispatcher; // :3

/**
 * @addtogroup input
 * @{
 * @js NA
 * @lua NA
 */ // :3

class CC_DLL CCTouchDelegate // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    CCTouchDelegate() {} // :3

    virtual ~CCTouchDelegate() // :3
    { // :3
    } // :3

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent); return false;}; // :3
    // optional :3

    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);} // :3
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);} // :3
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);} // :3

    // optional :3
     virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3

	virtual void setPreviousPriority(int); // :3
	virtual int getPreviousPriority(); // :3


}; // :3
/**
 @brief
 Using this type of delegate results in two benefits:
 - 1. You don't need to deal with CCSets, the dispatcher does the job of splitting
 them. You get exactly one UITouch per call.
 - 2. You can *claim* a UITouch by returning YES in ccTouchBegan. Updates of claimed
 touches are sent only to the delegate(s) that claimed them. So if you get a move/
 ended/canceled update you're sure it's your touch. This frees you from doing a
 lot of checks when doing multi-touch.

 (The name TargetedTouchDelegate relates to updates "targeting" their specific
 handler, without bothering the other handlers.)
 @since v0.8
 @js NA
 @lua NA
 */ // :3
 class CC_DLL CCTargetedTouchDelegate : public CCTouchDelegate // :3
 { // :3
 public: // :3
     /** Return YES to claim the touch.
      @since v0
     */ // :3
     virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) { CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);return false;}; // :3

     // optional :3
     virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);} // :3
 }; // :3

/** @brief
 This type of delegate is the same one used by CocoaTouch. You will receive all the events (Began,Moved,Ended,Canceled).
 @since v0.8
 @js NA
 @lua NA
 */ // :3
 class CC_DLL CCStandardTouchDelegate : public CCTouchDelegate // :3
 { // :3
 public: // :3
     // optional :3
     virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
     virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);} // :3
 }; // :3

// end of input group :3
/// @} :3

NS_CC_END // :3

#endif // __TOUCH_DISPATHCHER_CCTOUCH_DELEGATE_PROTOCOL_H__ :3
