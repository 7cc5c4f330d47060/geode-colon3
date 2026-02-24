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
#include "../cocoa/CCSet.h" // :3
#include "CCDirector.h" // :3
#include "keypad_dispatcher/CCKeypadDispatcher.h" // :3
#include "../touch_dispatcher/CCTouch.h" // :3
#include "../CCEGLView.h" // :3
#include "../touch_dispatcher/CCTouchDispatcher.h" // :3

#include <android/log.h> // :3
#include <jni.h> // :3

using namespace cocos2d; // :3

extern "C" { // :3
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y) { // :3
        cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesBegin(1, &id, &x, &y); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y) { // :3
        cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesEnd(1, &id, &x, &y); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) { // :3
        int size = env->GetArrayLength(ids); // :3
        jint id[size]; // :3
        jfloat x[size]; // :3
        jfloat y[size]; // :3

        env->GetIntArrayRegion(ids, 0, size, id); // :3
        env->GetFloatArrayRegion(xs, 0, size, x); // :3
        env->GetFloatArrayRegion(ys, 0, size, y); // :3

        cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesMove(size, id, x, y); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesCancel(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) { // :3
        int size = env->GetArrayLength(ids); // :3
        jint id[size]; // :3
        jfloat x[size]; // :3
        jfloat y[size]; // :3

        env->GetIntArrayRegion(ids, 0, size, id); // :3
        env->GetFloatArrayRegion(xs, 0, size, x); // :3
        env->GetFloatArrayRegion(ys, 0, size, y); // :3

        cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesCancel(size, id, x, y); // :3
    } // :3

    #define KEYCODE_BACK 0x04 // :3
    #define KEYCODE_MENU 0x52 // :3

    JNIEXPORT jboolean JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown(JNIEnv * env, jobject thiz, jint keyCode) { // :3
        CCDirector* pDirector = CCDirector::sharedDirector(); // :3
        switch (keyCode) { // :3
            case KEYCODE_BACK: // :3
                  if (pDirector->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked)) // :3
                    return JNI_TRUE; // :3
                break; // :3
            case KEYCODE_MENU: // :3
                if (pDirector->getKeypadDispatcher()->dispatchKeypadMSG(kTypeMenuClicked)) // :3
                    return JNI_TRUE; // :3
                break; // :3
            default: // :3
                return JNI_FALSE; // :3
        } // :3
        return JNI_FALSE; // :3
    } // :3
} // :3
