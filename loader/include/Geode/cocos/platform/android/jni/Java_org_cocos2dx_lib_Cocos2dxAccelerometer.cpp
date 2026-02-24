#include "../cocoa/CCGeometry.h" // :3
#include "platform/android/CCAccelerometer.h" // :3
#include "../CCEGLView.h" // :3
#include "JniHelper.h" // :3
#include <jni.h> // :3
#include "CCDirector.h" // :3

using namespace cocos2d; // :3

extern "C" { // :3
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxAccelerometer_onSensorChanged(JNIEnv*  env, jobject thiz, jfloat x, jfloat y, jfloat z, jlong timeStamp) { // :3
        CCDirector* pDirector = CCDirector::sharedDirector(); // :3
        pDirector->getAccelerometer()->update(x, y, z, timeStamp); // :3
    } // :3
} // :3
