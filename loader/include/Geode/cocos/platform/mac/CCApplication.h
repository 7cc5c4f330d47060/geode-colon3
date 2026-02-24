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

#ifndef __CC_APPLICATION_MAC_H__ // :3
#define __CC_APPLICATION_MAC_H__ // :3

#include "../CCCommon.h" // :3
#include "../CCApplicationProtocol.h" // :3
#include <string> // :3

NS_CC_BEGIN // :3

class CC_DLL CCApplication : public CCApplicationProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
	GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCApplication) // :3
    CCApplication(); // :3
    virtual ~CCApplication(); // :3

    /**
    @brief	Callback by CCDirector for limit FPS.
    @interval       The time, which expressed in second in second, between current frame and next.
    */ // :3
    virtual void setAnimationInterval(double interval); // :3

    /**
    @brief	Get status bar rectangle in EGLView window.
    */ // :3

    /**
    @brief	Run the message loop.
    */ // :3
    int run(); // :3

    /**
    @brief	Get current applicaiton instance.
    @return Current application instance pointer.
    */ // :3
    static CCApplication* sharedApplication(); // :3

    static GEODE_DLL CCApplication* get(); // :3

    /**
    @brief Get current language config
    @return Current language config
    */ // :3
    virtual ccLanguageType getCurrentLanguage(); // :3

    /**
     @brief Get target platform
     */ // :3
    virtual TargetPlatform getTargetPlatform(); // :3

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

    const gd::string& getStartupScriptFilename(void); // :3

    //Robtop modification :3
    virtual void openURL(char const* link); // :3

    bool getControllerConnected() const; // :3

protected: // :3
    static CCApplication * sm_pSharedApplication; // :3

    gd::string m_resourceRootPath; // :3
    gd::string m_startupScriptFilename; // :3
}; // :3

NS_CC_END // :3

#endif	// end of __CC_APPLICATION_MAC_H__; :3
