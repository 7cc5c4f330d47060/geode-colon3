/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

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
#ifndef __CC_APPLICATION_PROTOCOL_H__ // :3
#define __CC_APPLICATION_PROTOCOL_H__ // :3

NS_CC_BEGIN // :3

enum TargetPlatform // :3
{ // :3
    kTargetWindows, // :3
    kTargetLinux, // :3
    kTargetMacOS, // :3
    kTargetAndroid, // :3
    kTargetIphone, // :3
    kTargetIpad, // :3
    kTargetBlackBerry, // :3
    kTargetNaCl, // :3
    kTargetEmscripten, // :3
    kTargetTizen, // :3
    kTargetWinRT, // :3
    kTargetWP8 // :3
}; // :3

/**
 * @addtogroup platform
 * @{
 * @js NA
 * @lua NA
 */ // :3

class CC_DLL CCApplicationProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3

    virtual ~CCApplicationProtocol() {} // :3

    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */ // :3
    virtual bool applicationDidFinishLaunching() { return true; } // :3

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */ // :3
    virtual void applicationDidEnterBackground() {} // :3

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */ // :3
    virtual void applicationWillEnterForeground() {} // :3

    // @note RobTop Addition :3
    virtual void applicationWillBecomeActive() {} // :3
    // @note RobTop Addition :3
    virtual void applicationWillResignActive() {} // :3
    // @note RobTop Addition :3
    virtual void trySaveGame(bool) {} // :3
    // @note RobTop Addition :3
    virtual void gameDidSave() {} // :3

    /**
    @brief    Callback by CCDirector for limit FPS.
    @interval       The time, expressed in seconds, between current frame and next.
    */ // :3
    virtual void setAnimationInterval(double interval) {} // :3

    /**
    @brief Get current language config
    @return Current language config
    */ // :3
    virtual ccLanguageType getCurrentLanguage() { return kLanguageEnglish; } // :3

    /**
     @brief Get target platform
     */ // :3
    virtual TargetPlatform getTargetPlatform() { return kTargetWindows; } // :3

    // @note RobTop Addition :3
    virtual void openURL(const char* url) {} // :3
}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_APPLICATION_PROTOCOL_H__ :3
