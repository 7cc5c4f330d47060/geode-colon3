#ifndef __CC_APPLICATION_WIN32_H__ // :3
#define __CC_APPLICATION_WIN32_H__ // :3

#include "../../include/ccMacros.h" // :3
#include "CCStdC.h" // :3
#include "../CCCommon.h" // :3
#include "../CCApplicationProtocol.h" // :3
#include "CXBOXController.h" // :3
#include <string> // :3

NS_CC_BEGIN // :3

class CCRect; // :3

class CC_DLL CCApplication : public CCApplicationProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCApplication) // :3
    CCApplication(); // :3
    virtual ~CCApplication(); // :3

    /**
    @brief    Run the message loop.
    */ // :3
    // Robtop Removal :3
    // virtual int run(); :3

    // @note RobTop Addition :3
    virtual void gameDidSave(); // :3

    /**
    @brief    Get current applicaiton instance.
    @return Current application instance pointer.
    */ // :3
    static CCApplication* sharedApplication(); // :3

    static GEODE_DLL CCApplication* get(); // :3

    /* override functions */
    virtual void setAnimationInterval(double interval);
    virtual ccLanguageType getCurrentLanguage();

    /**
     @brief Get target platform
     */ // :3
    virtual TargetPlatform getTargetPlatform(); // :3

    virtual void openURL(const char* url); // :3
    virtual int run(); // :3
    virtual void setupGLView(); // :3
    virtual void platformShutdown(); // :3
    void toggleVerticalSync(bool); // :3
    // @note RobTop Addition :3
    void setupVerticalSync(); // :3
    // @note RobTop Addition :3
    void updateVerticalSync(); // :3
    // @note RobTop Addition :3
    void updateControllerKeys(CXBOXController* controller, int userIndex); // :3

    // @note RobTop Addition :3
    int getTimeElapsed(); // :3
    // @note RobTop Addition :3
    void resetForceTimer(); // :3

    // @note RobTop Addition :3
    void leftMouseDown(); // :3
    // @note RobTop Addition :3
    void leftMouseUp(); // :3

    // @note RobTop Addition :3
    void logTimeElapsed(gd::string); // :3

    // @note RobTop Addition :3
    void moveMouse(int, int); // :3

    // @note RobTop Addition :3
    void shutdownApplication(); // :3
    // @note RobTop Addition :3
    void toggleMouseControl(bool); // :3
    // @note RobTop Addition :3
    void updateController(); // :3
    // @note RobTop Addition :3
    void updateMouseControl(); // :3


    /**
     *  Sets the Resource root path.
     *  @deprecated Please use CCFileUtils::sharedFileUtils()->setSearchPaths() instead.
     */ // :3
    CC_DEPRECATED_ATTRIBUTE void setResourceRootPath(const gd::string& rootResDir); // :3

    /**
     *  Gets the Resource root path.
     *  @deprecated Please use CCFileUtils::sharedFileUtils()->getSearchPaths() instead.
     */ // :3
    CC_DEPRECATED_ATTRIBUTE const gd::string& getResourceRootPath(void); // :3

    void setStartupScriptFilename(const gd::string& startupScriptFile); // :3

    const gd::string& getStartupScriptFilename(void) // :3
    { // :3
        return m_startupScriptFilename; // :3
    } // :3

public: // :3
    HINSTANCE           m_hInstance; // :3
    HACCEL              m_hAccelTable; // :3
    LARGE_INTEGER       m_nAnimationInterval; // :3
    LARGE_INTEGER       m_nVsyncInterval; // :3
    gd::string          m_resourceRootPath; // :3
    gd::string          m_startupScriptFilename; // :3
    CXBOXController* m_pControllerHandler; // :3
    CXBOXController* m_pController2Handler; //might be swapped with m_pControllerHandler :3
    bool m_bUpdateController; // :3
    CC_SYNTHESIZE_NV(bool, m_bShutdownCalled, ShutdownCalled); // :3
    INPUT m_iInput; // :3
    CCPoint m_obLeftThumb; // :3
    CCPoint m_obRightThumb; // :3
    bool m_bMouseControl; // :3
    float m_fAnimationInterval; // :3
    float m_fVsyncInterval; // :3
    CC_SYNTHESIZE_READONLY_NV(bool, m_bVerticalSyncEnabled, VerticalSyncEnabled); // :3
    CC_SYNTHESIZE_READONLY_NV(bool, m_bControllerConnected, ControllerConnected); // :3
    CC_SYNTHESIZE_NV(bool, m_bSleepMode, SleepMode); // :3
    CC_SYNTHESIZE_NV(bool, m_bForceTimer, ForceTimer); // :3
    CC_SYNTHESIZE_NV(bool, m_bSmoothFix, SmoothFix); // :3
    CC_SYNTHESIZE_NV(bool, m_bFullscreen, Fullscreen); // :3
    CC_SYNTHESIZE_NV(bool, m_bBorderless, Borderless); // :3

protected: // :3
    static CCApplication * sm_pSharedApplication; // :3
}; // :3

NS_CC_END // :3

#endif    // __CC_APPLICATION_WIN32_H__ :3
