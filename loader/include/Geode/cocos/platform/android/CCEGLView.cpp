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
#include "CCEGLView.h" // :3
#include "../cocoa/CCSet.h" // :3
#include "CCDirector.h" // :3
#include "../include/ccMacros.h" // :3
#include "../touch_dispatcher/CCTouchDispatcher.h" // :3
#include "jni/IMEJni.h" // :3
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h" // :3
#include "../platform/CCGL.h" // :3

#include <stdlib.h> // :3
#include <android/log.h> // :3



#if CC_TEXTURE_ATLAS_USE_VAO // :3

// <EGL/egl.h> exists since android 2.3 :3
#include <EGL/egl.h> // :3
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT = 0; // :3
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT = 0; // :3
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT = 0; // :3

#endif // :3

void initExtensions() { // :3
#if CC_TEXTURE_ATLAS_USE_VAO // :3
     glGenVertexArraysOESEXT = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES"); // :3
     glBindVertexArrayOESEXT = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES"); // :3
     glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES"); // :3
#endif // :3
} // :3

NS_CC_BEGIN // :3

CCEGLView::CCEGLView() // :3
{ // :3
    initExtensions(); // :3
} // :3

CCEGLView::~CCEGLView() // :3
{ // :3

} // :3

bool CCEGLView::isOpenGLReady() // :3
{ // :3
    return (m_obScreenSize.width != 0 && m_obScreenSize.height != 0); // :3
} // :3

void CCEGLView::end() // :3
{ // :3
    terminateProcessJNI(); // :3
} // :3

void CCEGLView::swapBuffers() // :3
{ // :3
} // :3

CCEGLView* CCEGLView::sharedOpenGLView() // :3
{ // :3
    static CCEGLView instance; // :3
    return &instance; // :3
} // :3

void CCEGLView::setIMEKeyboardState(bool bOpen) // :3
{ // :3
    setKeyboardStateJNI((int)bOpen); // :3
} // :3

NS_CC_END // :3

