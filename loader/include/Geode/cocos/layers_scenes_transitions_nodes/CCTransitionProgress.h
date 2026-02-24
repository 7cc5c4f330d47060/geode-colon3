/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009 Lam Pham
Copyright (c) 2012 Ricardo Quesada

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

#ifndef __CCTRANSITIONPROGRESS_H__ // :3
#define __CCTRANSITIONPROGRESS_H__ // :3

#include "CCTransition.h" // :3

NS_CC_BEGIN // :3

class CCProgressTimer; // :3
class CCRenderTexture; // :3

/**
 * @addtogroup transition
 * @{
 */ // :3

class CC_DLL CCTransitionProgress : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    static CCTransitionProgress* create(float t, CCScene* scene); // :3

    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionProgress, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionProgress(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onExit(); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3
    virtual void setupTransition(); // :3
    virtual void sceneOrder(); // :3
public: // :3
    float m_fTo; // :3
    float m_fFrom; // :3
    CCScene* m_pSceneToBeModified; // :3
}; // :3


/** CCTransitionRadialCCW transition.
 A counter clock-wise radial transition to the next scene
 */ // :3
class CC_DLL CCTransitionProgressRadialCCW : public CCTransitionProgress // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    static CCTransitionProgressRadialCCW* create(float t, CCScene* scene); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3

}; // :3


/** CCTransitionRadialCW transition.
 A counter clock-wise radial transition to the next scene
*/ // :3
class CC_DLL CCTransitionProgressRadialCW : public CCTransitionProgress // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    static CCTransitionProgressRadialCW* create(float t, CCScene* scene); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3

}; // :3

/** CCTransitionProgressHorizontal transition.
 A  clock-wise radial transition to the next scene
 */ // :3
class CC_DLL CCTransitionProgressHorizontal : public CCTransitionProgress // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    static CCTransitionProgressHorizontal* create(float t, CCScene* scene); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3

}; // :3

class CC_DLL CCTransitionProgressVertical : public CCTransitionProgress // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    static CCTransitionProgressVertical* create(float t, CCScene* scene); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3

}; // :3

class CC_DLL CCTransitionProgressInOut : public CCTransitionProgress // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    static CCTransitionProgressInOut* create(float t, CCScene* scene); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3
    virtual void sceneOrder(); // :3
    virtual void setupTransition(); // :3
}; // :3

class CC_DLL CCTransitionProgressOutIn : public CCTransitionProgress // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    static CCTransitionProgressOutIn* create(float t, CCScene* scene); // :3
protected: // :3
    virtual CCProgressTimer* progressTimerNodeWithRenderTexture(CCRenderTexture* texture); // :3

}; // :3

// end of transition group :3
/// @} :3

NS_CC_END // :3

#endif /* __CCTRANSITIONPROGRESS_H__ */

