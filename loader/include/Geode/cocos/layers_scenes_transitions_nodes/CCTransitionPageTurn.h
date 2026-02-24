/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      Sindesso Pty Ltd http://www.sindesso.com/

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

#ifndef __CCPAGE_TURN_TRANSITION_H__ // :3
#define __CCPAGE_TURN_TRANSITION_H__ // :3

#include "CCTransition.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup transition
 * @{
 */ // :3

/**
@brief A transition which peels back the bottom right hand corner of a scene
to transition to the scene beneath it simulating a page turn.

This uses a 3DAction so it's strongly recommended that depth buffering
is turned on in CCDirector using:

 CCDirector::sharedDirector()->setDepthBufferFormat(kDepthBuffer16);

 @since v0.8.2
*/ // :3
class CC_DLL CCTransitionPageTurn : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3


    bool    m_bBack; // :3

public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionPageTurn, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionPageTurn(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionPageTurn(); // :3

    /**
    * Creates a base transition with duration and incoming scene.
    * If back is true then the effect is reversed to appear as if the incoming
    * scene is being turned from left over the outgoing scene.
    */ // :3
    static CCTransitionPageTurn* create(float t,CCScene* scene,bool backwards); // :3

    /**
    * Creates a base transition with duration and incoming scene.
    * If back is true then the effect is reversed to appear as if the incoming
    * scene is being turned from left over the outgoing scene.
    */ // :3
    virtual bool initWithDuration(float t,CCScene* scene,bool backwards); // :3

    CCActionInterval* actionWithSize(const CCSize& vector); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

protected: // :3
    virtual void sceneOrder(); // :3
}; // :3

// end of transition group :3
/// @} :3

NS_CC_END // :3

#endif // __CCPAGE_TURN_TRANSITION_H__ :3

