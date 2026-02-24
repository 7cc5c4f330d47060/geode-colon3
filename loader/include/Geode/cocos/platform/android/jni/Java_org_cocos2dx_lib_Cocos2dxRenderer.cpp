#include "text_input_node/CCIMEDispatcher.h" // :3
#include "CCDirector.h" // :3
#include "../CCApplication.h" // :3
#include "../platform/CCFileUtils.h" // :3
#include "CCEventType.h" // :3
#include "support/CCNotificationCenter.h" // :3
#include "JniHelper.h" // :3
#include <jni.h> // :3

using namespace cocos2d; // :3

extern "C" { // :3
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender(JNIEnv* env) { // :3
        cocos2d::CCDirector::sharedDirector()->mainLoop(); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeOnPause() { // :3
        CCApplication::sharedApplication()->applicationDidEnterBackground(); // :3

        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_BACKGROUND, NULL); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeOnResume() { // :3
        if (CCDirector::sharedDirector()->getOpenGLView()) { // :3
            CCApplication::sharedApplication()->applicationWillEnterForeground(); // :3
        } // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInsertText(JNIEnv* env, jobject thiz, jstring text) { // :3
        const char* pszText = env->GetStringUTFChars(text, NULL); // :3
        cocos2d::CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(pszText, strlen(pszText)); // :3
        env->ReleaseStringUTFChars(text, pszText); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeDeleteBackward(JNIEnv* env, jobject thiz) { // :3
        cocos2d::CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward(); // :3
    } // :3

    JNIEXPORT jstring JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeGetContentText() { // :3
        JNIEnv * env = 0; // :3

        if (JniHelper::getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK || ! env) { // :3
            return 0; // :3
        } // :3
        const char * pszText = cocos2d::CCIMEDispatcher::sharedDispatcher()->getContentText(); // :3
        return env->NewStringUTF(pszText); // :3
    } // :3
} // :3
