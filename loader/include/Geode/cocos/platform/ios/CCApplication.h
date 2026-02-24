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

#ifndef __CC_APPLICATION_IOS_H__ // :3
#define __CC_APPLICATION_IOS_H__ // :3

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
    /**
     *  @js NA
     *  @lua NA
     */ // :3
    virtual ~CCApplication(); // :3

    /**
     @brief    Run the message loop.
     @js NA
     @lua NA
    */ // :3
    int run(); // :3

    /**
    @brief    Get the current application instance.
    @return Current application instance pointer.
    @js getInstance
    */ // :3
    static CCApplication* sharedApplication(); // :3

    static GEODE_DLL CCApplication* get(); // :3

    /**
     @brief    Callback by CCDirector for limit FPS.
     @interval       The time, expressed in seconds, between current frame and next.
     @lua NA
     */ // :3
    virtual void setAnimationInterval(double interval); // :3

    /**
    @brief Get current language config
    @return Current language config
    */ // :3
    virtual ccLanguageType getCurrentLanguage(); // :3

    /**
     @brief Get target platform
     */ // :3
    virtual TargetPlatform getTargetPlatform(); // :3

    //Robtop modification :3
    virtual void openURL(char const* link); // :3

protected: // :3
    static CCApplication * sm_pSharedApplication; // :3
}; // :3

NS_CC_END // :3

#endif    // end of __CC_APPLICATION_IOS_H__ :3
