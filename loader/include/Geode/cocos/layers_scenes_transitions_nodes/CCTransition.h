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
****************************************************************************/ // :3

#ifndef __CCTRANSITION_H__ // :3
#define __CCTRANSITION_H__ // :3

#include "CCScene.h" // :3
#include "../include/ccTypes.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup transition
 * @{
 */ // :3

//static creation function macro :3
//c/c++ don't support object creation of using class name :3
//so, all classes need creation method. :3

class CCActionInterval; // :3
class CCNode; // :3

/** @brief CCTransitionEaseScene can ease the actions of the scene protocol.
@since v0.8.2
*/ // :3
class CC_DLL CCTransitionEaseScene// : public CCObject :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /** returns the Ease action that will be performed on a linear action.
    @since v0.8.2
    */ // :3
    virtual CCActionInterval * easeActionWithAction(CCActionInterval * action) = 0; // :3
}; // :3

/** Orientation Type used by some transitions
*/ // :3
typedef enum { // :3
    /// An horizontal orientation where the Left is nearer :3
	kCCTransitionOrientationLeftOver = 0, // :3
	/// An horizontal orientation where the Right is nearer :3
	kCCTransitionOrientationRightOver = 1, // :3
	/// A vertical orientation where the Up is nearer :3
	kCCTransitionOrientationUpOver = 0, // :3
	/// A vertical orientation where the Bottom is nearer :3
	kCCTransitionOrientationDownOver = 1, // :3

	// Deprecated :3
    //	kOrientationLeftOver = kCCTransitionOrientationLeftOver, :3
    //	kOrientationRightOver = kCCTransitionOrientationRightOver, :3
    //	kOrientationUpOver = kCCTransitionOrientationUpOver, :3
    //	kOrientationDownOver = kCCTransitionOrientationDownOver, :3
} tOrientation; // :3

/** @brief Base class for CCTransition scenes
*/ // :3
class CC_DLL CCTransitionScene : public CCScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3

public: // :3
    CCScene    * m_pInScene; // :3
    CCScene    * m_pOutScene; // :3
    float    m_fDuration; // :3
    bool    m_bIsInSceneOnTop; // :3
    bool    m_bIsSendCleanupToScene; // :3

public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionScene, CCScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionScene(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionScene(); // :3
    virtual void draw(); // :3
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
    virtual void cleanup(); // :3

    /** creates a base transition with duration and incoming scene */
    static CCTransitionScene * create(float t, CCScene *scene);

    /** initializes a transition with duration and incoming scene */
    virtual bool initWithDuration(float t,CCScene* scene);

    /** called after the transition finishes */
    void finish(void);

    /** used by some transitions to hide the outer scene */
    void hideOutShowIn(void);

protected:
    virtual void sceneOrder();
private:
    void setNewScene(float dt);

};

/** @brief A CCTransition that supports orientation like.
* Possible orientation: LeftOver, RightOver, UpOver, DownOver
*/ // :3
class CC_DLL CCTransitionSceneOriented : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3


    tOrientation m_eOrientation; // :3

public: // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSceneOriented(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSceneOriented, CCTransitionScene) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSceneOriented(); // :3

    /** creates a base transition with duration and incoming scene */
    static CCTransitionSceneOriented * create(float t,CCScene* scene, tOrientation orientation);

    /** initializes a transition with duration and incoming scene */
    virtual bool initWithDuration(float t,CCScene* scene,tOrientation orientation);
};

/** @brief CCTransitionRotoZoom:
Rotate and zoom out the outgoing scene, and then rotate and zoom in the incoming
*/ // :3
class CC_DLL CCTransitionRotoZoom : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionRotoZoom(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionRotoZoom, CCTransitionScene) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionRotoZoom(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionRotoZoom* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionJumpZoom:
Zoom out and jump the outgoing scene, and then jump and zoom in the incoming
*/ // :3
class CC_DLL CCTransitionJumpZoom : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionJumpZoom, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionJumpZoom(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionJumpZoom(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionJumpZoom* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionMoveInL:
Move in from to the left the incoming scene.
*/ // :3
class CC_DLL CCTransitionMoveInL : public CCTransitionScene, public CCTransitionEaseScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionMoveInL, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionMoveInL(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionMoveInL(); // :3
    /** initializes the scenes */
    virtual void initScenes(void);
    /** returns the action that will be performed */
    virtual CCActionInterval* action(void);

    virtual CCActionInterval* easeActionWithAction(CCActionInterval * action);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionMoveInL* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionMoveInR:
Move in from to the right the incoming scene.
*/ // :3
class CC_DLL CCTransitionMoveInR : public CCTransitionMoveInL // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionMoveInR, CCTransitionMoveInL) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionMoveInR(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionMoveInR(); // :3
    virtual void initScenes(); // :3

    static CCTransitionMoveInR* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionMoveInT:
Move in from to the top the incoming scene.
*/ // :3
class CC_DLL CCTransitionMoveInT : public CCTransitionMoveInL // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionMoveInT, CCTransitionMoveInL) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionMoveInT(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionMoveInT(); // :3
    virtual void initScenes(); // :3

    static CCTransitionMoveInT* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionMoveInB:
Move in from to the bottom the incoming scene.
*/ // :3
class CC_DLL CCTransitionMoveInB : public CCTransitionMoveInL // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionMoveInB, CCTransitionMoveInL) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionMoveInB(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionMoveInB(); // :3
    virtual void initScenes(); // :3

    static CCTransitionMoveInB* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionSlideInL:
Slide in the incoming scene from the left border.
*/ // :3
class CC_DLL CCTransitionSlideInL : public CCTransitionScene, public CCTransitionEaseScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSlideInL, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSlideInL(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSlideInL(); // :3

    /** initializes the scenes */
    virtual void initScenes(void);
    /** returns the action that will be performed by the incoming and outgoing scene */
    virtual CCActionInterval* action(void);
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    virtual CCActionInterval* easeActionWithAction(CCActionInterval * action); // :3

    static CCTransitionSlideInL* create(float t, CCScene* scene); // :3
protected: // :3
    virtual void sceneOrder(); // :3
}; // :3

/** @brief CCTransitionSlideInR:
Slide in the incoming scene from the right border.
*/ // :3
class CC_DLL CCTransitionSlideInR : public CCTransitionSlideInL // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSlideInR, CCTransitionSlideInL) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSlideInR(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSlideInR(); // :3

    /** initializes the scenes */
    virtual void initScenes(void);
    /** returns the action that will be performed by the incoming and outgoing scene */
    virtual CCActionInterval* action(void);

    static CCTransitionSlideInR* create(float t, CCScene* scene);
protected:
    virtual void sceneOrder();
};

/** @brief CCTransitionSlideInB:
Slide in the incoming scene from the bottom border.
*/ // :3
class CC_DLL CCTransitionSlideInB : public CCTransitionSlideInL // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSlideInB, CCTransitionSlideInL) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSlideInB(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSlideInB(); // :3

    /** initializes the scenes */
    virtual void initScenes(void);
    /** returns the action that will be performed by the incoming and outgoing scene */
    virtual CCActionInterval* action(void);

    static CCTransitionSlideInB* create(float t, CCScene* scene);
protected:
    virtual void sceneOrder();
};

/** @brief CCTransitionSlideInT:
Slide in the incoming scene from the top border.
*/ // :3
class CC_DLL CCTransitionSlideInT : public CCTransitionSlideInL // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSlideInT, CCTransitionSlideInL) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSlideInT(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSlideInT(); // :3

    /** initializes the scenes */
    virtual void initScenes(void);
    /** returns the action that will be performed by the incoming and outgoing scene */
    virtual CCActionInterval* action(void);

    static CCTransitionSlideInT* create(float t, CCScene* scene);
protected:
    virtual void sceneOrder();
};

/**
@brief Shrink the outgoing scene while grow the incoming scene
*/ // :3
class CC_DLL CCTransitionShrinkGrow : public CCTransitionScene , public CCTransitionEaseScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionShrinkGrow, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionShrinkGrow(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionShrinkGrow(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    virtual CCActionInterval* easeActionWithAction(CCActionInterval * action); // :3

    static CCTransitionShrinkGrow* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionFlipX:
Flips the screen horizontally.
The front face is the outgoing scene and the back face is the incoming scene.
*/ // :3
class CC_DLL CCTransitionFlipX : public CCTransitionSceneOriented // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFlipX, CCTransitionSceneOriented) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFlipX(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFlipX(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionFlipX* create(float t, CCScene* s, tOrientation o); // :3
    static CCTransitionFlipX* create(float t, CCScene* s); // :3
}; // :3

/** @brief CCTransitionFlipY:
Flips the screen vertically.
The front face is the outgoing scene and the back face is the incoming scene.
*/ // :3
class CC_DLL CCTransitionFlipY : public CCTransitionSceneOriented // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFlipY, CCTransitionSceneOriented) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFlipY(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFlipY(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionFlipY* create(float t, CCScene* s, tOrientation o); // :3
    static CCTransitionFlipY* create(float t, CCScene* s); // :3
}; // :3

/** @brief CCTransitionFlipAngular:
Flips the screen half horizontally and half vertically.
The front face is the outgoing scene and the back face is the incoming scene.
*/ // :3
class CC_DLL CCTransitionFlipAngular : public CCTransitionSceneOriented // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFlipAngular, CCTransitionSceneOriented) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFlipAngular(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFlipAngular(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionFlipAngular* create(float t, CCScene* s, tOrientation o); // :3
    static CCTransitionFlipAngular* create(float t, CCScene* s); // :3
}; // :3

/** @brief CCTransitionZoomFlipX:
Flips the screen horizontally doing a zoom out/in
The front face is the outgoing scene and the back face is the incoming scene.
*/ // :3
class CC_DLL CCTransitionZoomFlipX : public CCTransitionSceneOriented // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionZoomFlipX, CCTransitionSceneOriented) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionZoomFlipX(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionZoomFlipX(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionZoomFlipX* create(float t, CCScene* s, tOrientation o); // :3
    static CCTransitionZoomFlipX* create(float t, CCScene* s); // :3
}; // :3

/** @brief CCTransitionZoomFlipY:
Flips the screen vertically doing a little zooming out/in
The front face is the outgoing scene and the back face is the incoming scene.
*/ // :3
class CC_DLL CCTransitionZoomFlipY : public CCTransitionSceneOriented // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionZoomFlipY, CCTransitionSceneOriented) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionZoomFlipY(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionZoomFlipY(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionZoomFlipY* create(float t, CCScene* s, tOrientation o); // :3
    static CCTransitionZoomFlipY* create(float t, CCScene* s); // :3
}; // :3

/** @brief CCTransitionZoomFlipAngular:
Flips the screen half horizontally and half vertically doing a little zooming out/in.
The front face is the outgoing scene and the back face is the incoming scene.
*/ // :3
class CC_DLL CCTransitionZoomFlipAngular : public CCTransitionSceneOriented // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionZoomFlipAngular, CCTransitionSceneOriented) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionZoomFlipAngular(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionZoomFlipAngular(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3

    static CCTransitionZoomFlipAngular* create(float t, CCScene* s, tOrientation o); // :3
    static CCTransitionZoomFlipAngular* create(float t, CCScene* s); // :3
}; // :3

/** @brief CCTransitionFade:
Fade out the outgoing scene and then fade in the incoming scene.'''
*/ // :3
class CC_DLL CCTransitionFade : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3


    ccColor4B    m_tColor; // :3

public: // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFade(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFade, CCTransitionScene) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFade(); // :3

    /** creates the transition with a duration and with an RGB color
    * Example: FadeTransition::create(2, scene, ccc3(255,0,0); // red color
    */ // :3
    static CCTransitionFade* create(float duration,CCScene* scene, const ccColor3B& color); // :3
    static CCTransitionFade* create(float duration,CCScene* scene); // :3

    /** initializes the transition with a duration and with an RGB color */
    virtual bool initWithDuration(float t, CCScene*scene ,const ccColor3B& color);

    virtual bool initWithDuration(float t,CCScene* scene);
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
}; // :3

class CCRenderTexture; // :3
/**
@brief CCTransitionCrossFade:
Cross fades two scenes using the CCRenderTexture object.
*/ // :3
class CC_DLL CCTransitionCrossFade : public CCTransitionScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public : // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionCrossFade, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionCrossFade(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionCrossFade(); // :3

    virtual void draw(); // :3
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

public: // :3
    static CCTransitionCrossFade* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionTurnOffTiles:
Turn off the tiles of the outgoing scene in random order
*/ // :3
class CC_DLL CCTransitionTurnOffTiles : public CCTransitionScene ,public CCTransitionEaseScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public : // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionTurnOffTiles, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionTurnOffTiles(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionTurnOffTiles(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    virtual CCActionInterval * easeActionWithAction(CCActionInterval * action); // :3

public: // :3
    static CCTransitionTurnOffTiles* create(float t, CCScene* scene); // :3
protected: // :3
    virtual void sceneOrder(); // :3
}; // :3

/** @brief CCTransitionSplitCols:
The odd columns goes upwards while the even columns goes downwards.
*/ // :3
class CC_DLL CCTransitionSplitCols : public CCTransitionScene , public CCTransitionEaseScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSplitCols, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSplitCols(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSplitCols(); // :3

    virtual CCActionInterval* action(void); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    virtual CCActionInterval * easeActionWithAction(CCActionInterval * action); // :3

public: // :3

    static CCTransitionSplitCols* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionSplitRows:
The odd rows goes to the left while the even rows goes to the right.
*/ // :3
class CC_DLL CCTransitionSplitRows : public CCTransitionSplitCols // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionSplitRows, CCTransitionSplitCols) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionSplitRows(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionSplitRows(); // :3

    virtual CCActionInterval* action(void); // :3

public: // :3

    static CCTransitionSplitRows* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionFadeTR:
Fade the tiles of the outgoing scene from the left-bottom corner the to top-right corner.
*/ // :3
class CC_DLL CCTransitionFadeTR : public CCTransitionScene , public CCTransitionEaseScene // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFadeTR, CCTransitionScene) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFadeTR(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFadeTR(); // :3
    virtual CCActionInterval* actionWithSize(const CCSize& size); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual void onEnter(); // :3
    virtual CCActionInterval* easeActionWithAction(CCActionInterval * action); // :3

public: // :3

    static CCTransitionFadeTR* create(float t, CCScene* scene); // :3
protected: // :3
    virtual void sceneOrder(); // :3
}; // :3

/** @brief CCTransitionFadeBL:
Fade the tiles of the outgoing scene from the top-right corner to the bottom-left corner.
*/ // :3
class CC_DLL CCTransitionFadeBL : public CCTransitionFadeTR // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFadeBL, CCTransitionFadeTR) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFadeBL(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFadeBL(); // :3
    virtual CCActionInterval* actionWithSize(const CCSize& size); // :3

public: // :3

    static CCTransitionFadeBL* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionFadeUp:
* Fade the tiles of the outgoing scene from the bottom to the top.
*/ // :3
class CC_DLL CCTransitionFadeUp : public CCTransitionFadeTR // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFadeUp, CCTransitionFadeTR) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFadeUp(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFadeUp(); // :3
    virtual CCActionInterval* actionWithSize(const CCSize& size); // :3

public: // :3

    static CCTransitionFadeUp* create(float t, CCScene* scene); // :3
}; // :3

/** @brief CCTransitionFadeDown:
* Fade the tiles of the outgoing scene from the top to the bottom.
*/ // :3
class CC_DLL CCTransitionFadeDown : public CCTransitionFadeTR // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCTransitionFadeDown, CCTransitionFadeTR) // :3
    /**
     *  @js ctor
     */ // :3
    CCTransitionFadeDown(); // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCTransitionFadeDown(); // :3
    virtual CCActionInterval* actionWithSize(const CCSize& size); // :3

public: // :3

    static CCTransitionFadeDown* create(float t, CCScene* scene); // :3
}; // :3

// end of transition group :3
/// @} :3

NS_CC_END // :3

#endif // __CCTRANSITION_H__ :3

