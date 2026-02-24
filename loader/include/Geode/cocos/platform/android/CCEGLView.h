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

#ifndef __CC_EGLVIEW_ANDROID_H__ // :3
#define __CC_EGLVIEW_ANDROID_H__ // :3

#include "../../cocoa/CCGeometry.h" // :3
#include "../CCEGLViewProtocol.h" // :3

NS_CC_BEGIN // :3

class CC_DLL CCEGLView : public CCEGLViewProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCEGLView, CCEGLViewProtocol) // :3

    CCEGLView(); // :3
    virtual ~CCEGLView(); // :3

    bool    isOpenGLReady(); // :3

    // keep compatible :3
    void    end(); // :3
    void    swapBuffers(); // :3
    void    setIMEKeyboardState(bool bOpen); // :3

    // static function :3
    /**
    @brief    get the shared main open gl window
    */ // :3
    static CCEGLView* sharedOpenGLView(); // :3

    static GEODE_DLL CCEGLView* get(); // :3
}; // :3

NS_CC_END // :3

#endif    // end of __CC_EGLVIEW_ANDROID_H__ :3
