#ifndef __CCEGLVIEWPROTOCOL_H__ // :3
#define __CCEGLVIEWPROTOCOL_H__ // :3

#include "../include/ccTypes.h" // :3

enum ResolutionPolicy // :3
{ // :3
    // The entire application is visible in the specified area without trying to preserve the original aspect ratio. :3
    // Distortion can occur, and the application may appear stretched or compressed. :3
    kResolutionExactFit, // :3
    // The entire application fills the specified area, without distortion but possibly with some cropping, :3
    // while maintaining the original aspect ratio of the application. :3
    kResolutionNoBorder, // :3
    // The entire application is visible in the specified area without distortion while maintaining the original :3
    // aspect ratio of the application. Borders can appear on two sides of the application. :3
    kResolutionShowAll, // :3
    // The application takes the height of the design resolution size and modifies the width of the internal :3
    // canvas so that it fits the aspect ratio of the device :3
    // no distortion will occur however you must make sure your application works on different :3
    // aspect ratios :3
    kResolutionFixedHeight, // :3
    // The application takes the width of the design resolution size and modifies the height of the internal :3
    // canvas so that it fits the aspect ratio of the device :3
    // no distortion will occur however you must make sure your application works on different :3
    // aspect ratios :3
    kResolutionFixedWidth, // :3

    kResolutionUnKnown, // :3
}; // :3

NS_CC_BEGIN // :3

#define CC_MAX_TOUCHES  5 // :3

class EGLTouchDelegate; // :3
class CCSet; // :3

/**
 * @addtogroup platform
 * @{
 * @js NA
 */ // :3

class CC_DLL CCEGLViewProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @lua NA
     */ // :3
    CCEGLViewProtocol(); // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCEGLViewProtocol) // :3
    /**
     * @lua NA
     */ // :3
    virtual ~CCEGLViewProtocol(); // :3

    /** Force destroying EGL view, subclass must implement this method.
     * @lua NA
     */ // :3
    virtual void    end() {} // :3

    /** Get whether opengl render system is ready, subclass must implement this method.
     * @lua NA
     */ // :3
    virtual bool    isOpenGLReady() { return false; } // :3

    /** Exchanges the front and back buffers, subclass must implement this method.
     * @lua NA
     */ // :3
    virtual void    swapBuffers() {} // :3

    /** Open or close IME keyboard , subclass must implement this method.
     * @lua NA
     */ // :3
    virtual void    setIMEKeyboardState(bool bOpen) {} // :3

    /**
     * Get the frame size of EGL view.
     * In general, it returns the screen size since the EGL view is a fullscreen view.
     */ // :3
    virtual const CCSize& getFrameSize() const; // :3

    /**
     * Set the frame size of EGL view.
     */ // :3
    virtual void setFrameSize(float width, float height); // :3

    /**
     * Get the visible area size of opengl viewport.
     */ // :3
    virtual CCSize getVisibleSize() const; // :3

    /**
     * Get the visible origin point of opengl viewport.
     */ // :3
    virtual CCPoint getVisibleOrigin() const; // :3

    /**
     * Set the design resolution size.
     * @param width Design resolution width.
     * @param height Design resolution height.
     * @param resolutionPolicy The resolution policy desired, you may choose:
     *                         [1] kResolutionExactFit Fill screen by stretch-to-fit: if the design resolution ratio of width to height is different from the screen resolution ratio, your game view will be stretched.
     *                         [2] kResolutionNoBorder Full screen without black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two areas of your game view will be cut.
     *                         [3] kResolutionShowAll  Full screen with black border: if the design resolution ratio of width to height is different from the screen resolution ratio, two black borders will be shown.
     */ // :3
    virtual void setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy); // :3

    /** Get design resolution size.
     *  Default resolution size is the same as 'getFrameSize'.
     */ // :3
    virtual const CCSize&  getDesignResolutionSize() const; // :3

    /** Set touch delegate */
    virtual void setTouchDelegate(EGLTouchDelegate * pDelegate);

    /**
     * Set opengl view port rectangle with points.
     */ // :3
    virtual void setViewPortInPoints(float x , float y , float w , float h); // :3

    /**
     * Set Scissor rectangle with points.
     */ // :3
    virtual void setScissorInPoints(float x , float y , float w , float h); // :3

    /**
     * Get whether GL_SCISSOR_TEST is enable
     * @lua NA
     */ // :3
    virtual bool isScissorEnabled(); // :3

    /**
     * Get the current scissor rectangle
     * @lua NA
     */ // :3
    virtual CCRect getScissorRect(); // :3
    /**
     * @lua NA
     */ // :3
    virtual void setViewName(const char* pszViewName); // :3
    /**
     * @lua NA
     */ // :3
    const char* getViewName(); // :3

    /** Touch events are handled by default; if you want to customize your handlers, please override these functions:
     * @lua NA
     */ // :3
    virtual void handleTouchesBegin(int num, int ids[], float xs[], float ys[], double timestamp); // :3
    /**
     * @lua NA
     */ // :3
    virtual void handleTouchesMove(int num, int ids[], float xs[], float ys[], double timestamp); // :3
    /**
     * @lua NA
     */ // :3
    virtual void handleTouchesEnd(int num, int ids[], float xs[], float ys[], double timestamp); // :3
    /**
     * @lua NA
     */ // :3
    virtual void handleTouchesCancel(int num, int ids[], float xs[], float ys[], double timestamp); // :3

    /**
     * Get the opengl view port rectangle.
     */ // :3
    const CCRect& getViewPortRect() const; // :3

    /**
     * Get scale factor of the horizontal direction.
     */ // :3
    float getScaleX() const; // :3

    /**
     * Get scale factor of the vertical direction.
     */ // :3
    float getScaleY() const; // :3

	virtual void pollInputEvents(); // :3
	void updateDesignResolutionSize(); // :3

    // @note RobTop Addition :3
    double getPlatformTimestamp(); // :3

private: // :3
    void getSetOfTouchesEndOrCancel(CCSet& set, int num, int ids[], float xs[], float ys[], double timestamp); // :3

public: // :3
    EGLTouchDelegate* m_pDelegate; // :3

    // real screen size :3
    CCSize m_obScreenSize; // :3
    // resolution size, it is the size appropriate for the app resources. :3
    CCSize m_obDesignResolutionSize; // :3
    // the view port size :3
    CCRect m_obViewPortRect; // :3
    // the view name :3
    char   m_szViewName[50]; // :3

    float  m_fScaleX; // :3
    float  m_fScaleY; // :3
    ResolutionPolicy m_eResolutionPolicy; // :3

    // RobTop addition, seemingly unused and set to 1.0 in ctor :3
    float  m_unkFloat; // :3
}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif /* __CCEGLVIEWPROTOCOL_H__ */
