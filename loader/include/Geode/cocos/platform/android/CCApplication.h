#ifndef __CC_APPLICATION_ANDROID_H__ // :3
#define __CC_APPLICATION_ANDROID_H__ // :3

#include "../CCCommon.h" // :3
#include "../CCApplicationProtocol.h" // :3

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
    @brief    Callback by CCDirector to limit FPS.
    @interval       The time, expressed in seconds, between current frame and next.
    */ // :3
    virtual void setAnimationInterval(double interval) override; // :3

    /**
    @brief    Run the message loop.
    */ // :3
    int run(); // :3

    /**
    @brief    Get current application instance.
    @return Current application instance pointer.
    */ // :3
    static CCApplication* sharedApplication(); // :3

    static GEODE_DLL CCApplication* get(); // :3

    /**
    @brief Get current language config
    @return Current language config
    */ // :3
    virtual ccLanguageType getCurrentLanguage() override; // :3

    /**
     @brief Get target platform
     */ // :3
    virtual TargetPlatform getTargetPlatform() override; // :3

    virtual void openURL(char const* url) override; // :3

protected: // :3
    static CCApplication * sm_pSharedApplication; // :3
}; // :3

NS_CC_END // :3

#endif    // __CC_APPLICATION_ANDROID_H__ :3
