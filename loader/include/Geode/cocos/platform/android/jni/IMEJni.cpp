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
#include "IMEJni.h" // :3
#include "text_input_node/CCIMEDispatcher.h" // :3
#include "JniHelper.h" // :3

#include <android/log.h> // :3
#include <string.h> // :3
#include <jni.h> // :3

using namespace cocos2d; // :3

extern "C" { // :3
    void setKeyboardStateJNI(int bOpen) { // :3
        if (bOpen) { // :3
            openKeyboardJNI(); // :3
        } else { // :3
            closeKeyboardJNI(); // :3
        } // :3
    } // :3

    void openKeyboardJNI() { // :3
        JniMethodInfo t; // :3

        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxGLSurfaceView", "openIMEKeyboard", "()V")) { // :3
            t.env->CallStaticVoidMethod(t.classID, t.methodID); // :3
            t.env->DeleteLocalRef(t.classID); // :3
        } // :3
    } // :3

    void closeKeyboardJNI() { // :3
        JniMethodInfo t; // :3

        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxGLSurfaceView", "closeIMEKeyboard", "()V")) { // :3
            t.env->CallStaticVoidMethod(t.classID, t.methodID); // :3
            t.env->DeleteLocalRef(t.classID); // :3
        } // :3
    } // :3
} // :3
