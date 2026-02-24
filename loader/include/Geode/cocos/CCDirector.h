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

#ifndef __CCDIRECTOR_H__ // :3
#define __CCDIRECTOR_H__ // :3

#include "platform/CCPlatformMacros.h" // :3
#include "cocoa/CCObject.h" // :3
#include "include/ccTypes.h" // :3
#include "cocoa/CCGeometry.h" // :3
#include "cocoa/CCArray.h" // :3
#include "platform/CCGL.h" // :3
#include "kazmath/include/kazmath/mat4.h" // :3
#include "label_nodes/CCLabelAtlas.h" // :3
#include "include/ccTypeInfo.h" // :3


NS_CC_BEGIN // :3

/**
 * @addtogroup base_nodes
 * @{
 */ // :3

/** @typedef ccDirectorProjection
 Possible OpenGL projections used by director
 */ // :3
typedef enum { // :3
    /// sets a 2D projection (orthogonal projection) :3
    kCCDirectorProjection2D, // :3

    /// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500. :3
    kCCDirectorProjection3D, // :3

    /// it calls "updateProjection" on the projection delegate. :3
    kCCDirectorProjectionCustom, // :3

    /// Default projection is 3D projection :3
    kCCDirectorProjectionDefault = kCCDirectorProjection3D, // :3
} ccDirectorProjection; // :3

// @note RobTop Addition :3
typedef enum { // :3
    kTextureQualityLow = 1, // :3
    kTextureQualityMedium, // :3
    kTextureQualityHigh // :3
} TextureQuality; // :3

// @note RobTop Addition :3
typedef enum { // :3
    kPopTransitionFade, // :3
    kPopTransitionMoveInT // :3
} PopTransition; // :3


/* Forward declarations. */
class CCLabelAtlas;
class CCScene;
class CCEGLView;
class CCDirectorDelegate;
class CCNode;
class CCScheduler;
class CCActionManager;
class CCTouchDispatcher;
class CCKeypadDispatcher;
class CCAccelerometer;
// @note RobTop Addition
class CCKeyboardDispatcher;
// @note RobTop Addition
class CCMouseDispatcher;
// @note RobTop Addition
class CCSceneDelegate;
// @note RobTop Addition
class CCLabelBMFont;

/**
@brief Class that creates and handle the main Window and manages how
and when to execute the Scenes.

 The CCDirector is also responsible for:
  - initializing the OpenGL context
  - setting the OpenGL pixel format (default on is RGB565)
  - setting the OpenGL buffer depth (default one is 0-bit)
  - setting the projection (default one is 3D)
  - setting the orientation (default one is Portrait)

 Since the CCDirector is a singleton, the standard way to use it is by calling:
  _ CCDirector::sharedDirector()->methodName();

 The CCDirector also sets the default OpenGL context:
  - GL_TEXTURE_2D is enabled
  - GL_VERTEX_ARRAY is enabled
  - GL_COLOR_ARRAY is enabled
  - GL_TEXTURE_COORD_ARRAY is enabled
*/ // :3
class CC_DLL CCDirector : public CCObject, public TypeInfo // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     *  @js ctor
     */ // :3
    CCDirector(void); // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCDirector, CCObject) // :3
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCDirector(void); // :3
    virtual bool init(void); // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual long getClassTypeInfo() { // :3
		static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCDirector).name()); // :3
		return id; // :3
    } // :3

    static GEODE_DLL CCDirector* get(); // :3

    // attribute :3

    /** Get current running Scene. Director can only run one Scene at the time */
    inline CCScene* getRunningScene(void) { return m_pRunningScene; }

    /** Get the FPS value */
    inline double getAnimationInterval(void) { return m_dAnimationInterval; }
    /** Set the FPS value. */
    virtual void setAnimationInterval(double dValue) {}

    /** Whether or not to display the FPS on the bottom-left corner */
    inline bool isDisplayStats(void) { return m_bDisplayStats; }
    /** Display the FPS on the bottom-left corner */
    inline void setDisplayStats(bool bDisplayStats) { m_bDisplayStats = bDisplayStats; }

    /** seconds per frame */
    inline float getSecondsPerFrame() { return m_fSecondsPerFrame; }

    /** Get the CCEGLView, where everything is rendered
     * @js NA
     */ // :3
    inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; } // :3
    void setOpenGLView(CCEGLView *pobOpenGLView); // :3

    inline bool isNextDeltaTimeZero(void) { return m_bNextDeltaTimeZero; } // :3
    void setNextDeltaTimeZero(bool bNextDeltaTimeZero); // :3

    /** Whether or not the Director is paused */
    inline bool isPaused(void) { return m_bPaused; }

    // geode addition
    inline void setPaused(bool p) { m_bPaused = p; }

    /** How many frames were called since the director started */
    inline unsigned int getTotalFrames(void) { return m_uTotalFrames; }

    /** Sets an OpenGL projection
     @since v0.8.2
     @js NA
     */ // :3
    inline ccDirectorProjection getProjection(void) { return m_eProjection; } // :3
    void setProjection(ccDirectorProjection kProjection); // :3
     /** reshape projection matrix when canvas has been change"*/
    void reshapeProjection(const CCSize& newWindowSize);

    /** Sets the glViewport*/
    void setViewport();

    /** How many frames were called since the director started */


    /** Whether or not the replaced scene will receive the cleanup message.
     If the new scene is pushed, then the old scene won't receive the "cleanup" message.
     If the new scene replaces the old one, the it will receive the "cleanup" message.
     @since v0.99.0
     */ // :3
    inline bool isSendCleanupToScene(void) { return m_bSendCleanupToScene; } // :3

    /** This object will be visited after the main scene is visited.
     This object MUST implement the "visit" selector.
     Useful to hook a notification object, like CCNotifications (http://github.com/manucorporat/CCNotifications)
     @since v0.99.5
     */ // :3
    CCNode* getNotificationNode(); // :3
    [[deprecated("Use geode::OverlayManager instead")]] // :3
    void setNotificationNode(CCNode*); // :3

    /** CCDirector delegate. It shall implemente the CCDirectorDelegate protocol
     @since v0.99.5
     */ // :3
    CCDirectorDelegate* getDelegate() const; // :3
    void setDelegate(CCDirectorDelegate* pDelegate); // :3

    // window size :3

    /** returns the size of the OpenGL view in points.
    */ // :3
    CCSize getWinSize(void); // :3

    /** returns the size of the OpenGL view in pixels.
    */ // :3
    CCSize getWinSizeInPixels(void); // :3

    /** returns visible size of the OpenGL view in points.
     *  the value is equal to getWinSize if don't invoke
     *  CCEGLView::setDesignResolutionSize()
     */ // :3
    CCSize getVisibleSize(); // :3

    /** returns visible origin of the OpenGL view in points.
     */ // :3
    CCPoint getVisibleOrigin(); // :3

    /** converts a UIKit coordinate to an OpenGL coordinate
     Useful to convert (multi) touch coordinates to the current layout (portrait or landscape)
     */ // :3
    CCPoint convertToGL(const CCPoint& obPoint); // :3

    /** converts an OpenGL coordinate to a UIKit coordinate
     Useful to convert node points to window points for calls such as glScissor
     */ // :3
    CCPoint convertToUI(const CCPoint& obPoint); // :3

    /// XXX: missing description :3
    float getZEye(void); // :3

    // Scene Management :3

    /** Enters the Director's main loop with the given Scene.
     * Call it to run only your FIRST scene.
     * Don't call it if there is already a running scene.
     *
     * It will call pushScene: and then it will call startAnimation
     */ // :3
    void runWithScene(CCScene *pScene); // :3

    /** Suspends the execution of the running scene, pushing it on the stack of suspended scenes.
     * The new scene will be executed.
     * Try to avoid big stacks of pushed scenes to reduce memory allocation.
     * ONLY call it if there is a running scene.
     *
     * @note Robtop Addition: return value from void to bool
     */ // :3
    bool pushScene(CCScene *pScene); // :3

    /** Pops out a scene from the queue.
     * This scene will replace the running one.
     * The running scene will be deleted. If there are no more scenes in the stack the execution is terminated.
     * ONLY call it if there is a running scene.
     */ // :3
    void popScene(void); // :3

    /** Pops out all scenes from the queue until the root scene in the queue.
     * This scene will replace the running one.
     * Internally it will call `popToSceneStackLevel(1)`
     */ // :3
    void popToRootScene(void); // :3

    /** Pops out all scenes from the queue until it reaches `level`.
     If level is 0, it will end the director.
     If level is 1, it will pop all scenes until it reaches to root scene.
     If level is <= than the current stack level, it won't do anything.
     */ // :3
 	void popToSceneStackLevel(int level); // :3

    /** Replaces the running scene with a new one. The running scene is terminated.
     * ONLY call it if there is a running scene.
     *
     * @note Robtop Addition: return value from void to bool
     */ // :3
    bool replaceScene(CCScene *pScene); // :3

    /** Ends the execution, releases the running scene.
     It doesn't remove the OpenGL view from its parent. You have to do it manually.
     */ // :3
    void end(void); // :3

    /** Pauses the running scene.
     The running scene will be _drawed_ but all scheduled timers will be paused
     While paused, the draw rate will be 4 FPS to reduce CPU consumption
     */ // :3
    void pause(void); // :3

    /** Resumes the paused scene
     The scheduled timers will be activated again.
     The "delta time" will be 0 (as if the game wasn't paused)
     */ // :3
    void resume(void); // :3

protected: // :3
    /** Stops the animation. Nothing will be drawn. The main loop won't be triggered anymore.
     If you don't want to pause your animation call [pause] instead.
     */ // :3
    virtual void stopAnimation(void) {} // :3

    /** The main loop is triggered again.
     Call this function only if [stopAnimation] was called earlier
     @warning Don't call this function to start the main loop. To run the main loop call runWithScene
     */ // :3
    virtual void startAnimation(void) {} // :3

public: // :3
    /** Draw the scene.
    This method is called every frame. Don't call it manually.
    */ // :3
    void drawScene(void); // :3

    // Memory Helper :3

    /** Removes cached all cocos2d cached data.
     It will purge the CCTextureCache, CCSpriteFrameCache, CCLabelBMFont cache
     @since v0.99.3
     */ // :3
    void purgeCachedData(void); // :3

	/** sets the default values based on the CCConfiguration info */
    void setDefaultValues(void);

    // OpenGL Helper

    /** sets the OpenGL default values */
    void setGLDefaultValues(void);

    /** enables/disables OpenGL alpha blending */
    void setAlphaBlending(bool bOn);

    /** enables/disables OpenGL depth test */
    void setDepthTest(bool bOn);

protected:
    virtual void mainLoop(void) {}

public:
    /** The size in pixels of the surface. It could be different than the screen size.
    High-res devices might have a higher surface size than the screen size.
    Only available when compiled using SDK >= 4.0.
    @since v0.99.4
    */ // :3
    void setContentScaleFactor(float scaleFactor); // :3
    inline float getContentScaleFactor(void) { return m_fContentScaleFactor; } // :3

public: // :3
    // @note RobTop Addition :3
    void checkSceneReference(void); // :3
    // @note RobTop Addition :3
    inline CCScene* getNextScene(void) { return m_pNextScene; } // :3
    // @note RobTop Addition :3
    int levelForSceneInStack(CCScene*); // :3
    // @note RobTop Addition :3
    bool popSceneWithTransition(float, PopTransition); // :3
    // @note RobTop Addition :3
    void popToSceneInStack(CCScene*); // :3
    // @note RobTop Addition :3
    int sceneCount(void); // :3
    // @note RobTop Addition :3
    void willSwitchToScene(CCScene*); // :3

    // @note RobTop Addition :3
    void removeStatsLabel(void); // :3

    // @note RobTop Addition :3
    void resetSmoothFixCounter(void); // :3
    // @note RobTop Addition :3
    void setDeltaTime(float); // :3

    // @note RobTop Addition :3
    void setupScreenScale(CCSize, CCSize, TextureQuality); // :3
    // @note RobTop Addition :3
    void updateContentScale(TextureQuality); // :3
    // @note RobTop Addition :3
    void updateScreenScale(CCSize); // :3

    // @note RobTop Addition :3
    void applySmoothFix(); // :3
    // @note RobTop Addition :3
    void showFPSLabel(); // :3
    // @note RobTop Addition :3
    void toggleShowFPS(bool, gd::string, cocos2d::CCPoint); // :3
protected: // :3
    // @note RobTop Addition :3
    void createStatsLabel(); // :3

protected: // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactor, ScreenScaleFactor); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactorMax, ScreenScaleFactorMax); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactorW, ScreenScaleFactorW); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactorH, ScreenScaleFactorH); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(float, m_fScreenTop, ScreenTop); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV_NC(float, m_fScreenBottom, ScreenBottom); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV_NC(float, m_fScreenLeft, ScreenLeft); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(float, m_fScreenRight, ScreenRight); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(CCScene*, m_pSceneReference, SceneReference); // :3

public: // :3
    /** CCScheduler associated with this director
     @since v2.0
     */ // :3
    CC_PROPERTY(CCScheduler*, m_pScheduler, Scheduler); // :3

    /** CCActionManager associated with this director
     @since v2.0
     */ // :3
    CC_PROPERTY(CCActionManager*, m_pActionManager, ActionManager); // :3

    /** CCTouchDispatcher associated with this director
     @since v2.0
     */ // :3
    CC_PROPERTY(CCTouchDispatcher*, m_pTouchDispatcher, TouchDispatcher); // :3

    /** CCKeypadDispatcher associated with this director
     @since v2.0
     */ // :3
    CC_PROPERTY(CCKeypadDispatcher*, m_pKeypadDispatcher, KeypadDispatcher); // :3

    // @note RobTop Addition :3
    CC_PROPERTY(CCKeyboardDispatcher*, m_pKeyboardDispatcher, KeyboardDispatcher); // :3

    // @note RobTop Addition :3
    CC_PROPERTY(CCMouseDispatcher*, m_pMouseDispatcher, MouseDispatcher); // :3

    /** CCAccelerometer associated with this director
     @since v2.0
     @js NA
     @lua NA
     */ // :3
    CC_PROPERTY(CCAccelerometer*, m_pAccelerometer, Accelerometer); // :3

    /* delta time since last tick to main loop */
	CC_PROPERTY_READONLY(float, m_fDeltaTime, DeltaTime);

    /* *actual* delta time, according to rob. not sure what that means but i'm not arguing */
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(float, m_fActualDeltaTime, ActualDeltaTime);


public:
    /** returns a shared instance of the director
     *  @js getInstance
     */ // :3
    static CCDirector* sharedDirector(void); // :3

protected: // :3

    void purgeDirector(); // :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(bool, m_bIsTransitioning, IsTransitioning);   // if in a CCTransitionScene :3

    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(bool, m_bSmoothFix, SmoothFix);                        // if smooth fix is on :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(bool, m_bSmoothFixCheck, SmoothFixCheck);              // not exactly sure what this is, but the name says something ig :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_NV(bool, m_bForceSmoothFix, ForceSmoothFix);              // if "force smooth fix" is on or not :3
    // @note RobTop Addition :3
    CC_SYNTHESIZE_READONLY_NV(int, m_nSmoothFixCounter, SmoothFixCounter);  // not sure about this one either :3

    bool m_bPurgeDirecotorInNextLoop; // this flag will be set to true in end() :3

protected: // :3
    void setNextScene(void); // :3

    void showStats(); // :3
    // Robtop Removal :3
    // void createStatsLabel(); :3
    void calculateMPF(); // :3
    void getFPSImageData(unsigned char** datapointer, unsigned int* length); // :3

    /** calculates delta time since last time it was called */
    void calculateDeltaTime();
public:
    /* The CCEGLView, where everything is rendered */
    CCEGLView    *m_pobOpenGLView;

    double m_dAnimationInterval;
    double m_dOldAnimationInterval;

    /* landscape mode ? */
    bool m_bLandscape;

    bool m_bDisplayStats;

    float m_fFpsAccumDt;

    float m_fAccumDt;
    float m_fFrameRate;

    CCLabelAtlas *m_pFPSLabel;
    CCLabelAtlas *m_pSPFLabel;
    CCLabelAtlas *m_pDrawsLabel;

    /** Whether or not the Director is paused */
    bool m_bPaused;

    /* How many frames were called since the director started */
    unsigned int m_uTotalFrames;
    unsigned int m_uFrames;
    float m_fSecondsPerFrame;

    /* The running scene */
    CCScene *m_pRunningScene;

    /* will be the next 'runningScene' in the next frame
     nextScene is a weak reference. */ // :3
    CCScene *m_pNextScene; // :3

    /* If YES, then "old" scene will receive the cleanup message */
    bool    m_bSendCleanupToScene;

    /* scheduled scenes */
    CCArray* m_pobScenesStack;

    /* last time the main loop was updated */
    struct cc_timeval *m_pLastUpdate;

    /* whether or not the next delta time will be zero */
    bool m_bNextDeltaTimeZero;

    /* projection used */
    ccDirectorProjection m_eProjection;

    /* window size in points */
    CCSize m_obWinSizeInPoints;

    /* content scale factor */
    float    m_fContentScaleFactor;

    /* store the fps string */
    char *m_pszFPS;

    /* This object will be visited after the scene. Useful to hook a notification node */
    CCNode *m_pNotificationNode;

    /* Projection protocol delegate */
    CCDirectorDelegate *m_pProjectionDelegate;

    // @note RobTop Addition
    CC_SYNTHESIZE(CCSceneDelegate*, m_pAppDelegate, SceneDelegate);
    // @note RobTop Addition
    bool m_bDisplayFPS;
    // @note RobTop Addition
    CCLabelBMFont* m_pFPSNode;
    // @note RobTop Addition
    CCSize m_obScaleFactor;
    // @note RobTop Addition
    CCSize m_obResolutionInPixels;
    // @note RobTop Addition
    CC_SYNTHESIZE_READONLY_NV(TextureQuality, m_eTextureQuality, LoadedTextureQuality);
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(bool, m_bDontCallWillSwitch, DontCallWillSwitch);

#if GEODE_COMP_GD_VERSION >= 22030
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(bool, m_bFastMenu, FastMenu);
#endif

    // CCEGLViewProtocol will recreate stats labels to fit visible rect
    friend class CCEGLViewProtocol;
};

/**
 @brief DisplayLinkDirector is a Director that synchronizes timers with the refresh rate of the display.

 Features and Limitations:
  - Scheduled timers & drawing are synchronizes with the refresh rate of the display
  - Only supports animation intervals of 1/60 1/30 & 1/15

 @since v0.8.2
 @js NA
 @lua NA
 */ // :3
class CCDisplayLinkDirector : public CCDirector // :3
{ // :3
public: // :3
    CCDisplayLinkDirector(void) // :3
        : m_bInvalid(false) // :3
    {} // :3

    virtual void mainLoop(void); // :3
    virtual void setAnimationInterval(double dValue); // :3
    virtual void startAnimation(void); // :3
    virtual void stopAnimation(); // :3

protected: // :3
    bool m_bInvalid; // :3
}; // :3

// end of base_node group :3
/// @} :3

NS_CC_END // :3

#endif // __CCDIRECTOR_H__ :3
