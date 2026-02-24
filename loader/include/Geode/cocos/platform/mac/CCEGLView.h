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

#ifndef __CC_EGLVIEW_MAC_H__ // :3
#define __CC_EGLVIEW_MAC_H__ // :3

#include "../CCCommon.h" // :3
#include "../CCEGLViewProtocol.h" // :3

NS_CC_BEGIN // :3

class CCSet; // :3
class CCTouch; // :3
class CCSize; // :3

class CC_DLL CCEGLView : public CCEGLViewProtocol // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    static CCEGLView* sharedOpenGLView(void); // :3

    static GEODE_DLL CCEGLView* get(); // :3

    virtual ~CCEGLView(void); // :3

    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCEGLView, CCEGLViewProtocol) // :3

    virtual bool isOpenGLReady(void); // :3
    virtual bool setContentScaleFactor(float contentScaleFactor); // :3
    virtual void end(); // :3
    virtual void swapBuffers(void); // :3
    /**
     * Set opengl view port rectangle with points.
     */ // :3
    virtual void setViewPortInPoints(float x , float y , float w , float h); // :3
    virtual void setScissorInPoints(float x , float y , float w , float h); // :3

    virtual void setIMEKeyboardState(bool bOpen); // :3
	virtual void setMultiTouchMask(bool mask); // :3

private: // :3
    static CCEGLView* s_sharedView; // :3

    // @note RobTop Addition :3
    bool m_idk1; // :3
    // @note RobTop Addition :3
    bool m_idk2; // :3
    // @note RobTop Addition :3
    bool m_idk3; // :3

    CCEGLView(void); // :3
}; // :3

NS_CC_END   // end of namespace   cocos2d :3

#endif	// end of __CC_EGLVIEW_MAC_H__ :3
