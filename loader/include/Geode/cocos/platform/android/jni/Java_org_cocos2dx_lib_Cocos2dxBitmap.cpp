#include "JniHelper.h" // :3
#include <string.h> // :3
#include "CCDirector.h" // :3
#include "../CCApplication.h" // :3
#include "../platform/CCFileUtils.h" // :3
#include "CCEventType.h" // :3
#include "support/CCNotificationCenter.h" // :3
#include <jni.h> // :3

using namespace cocos2d; // :3


int getFontSizeAccordingHeightJni(int height) { // :3
    int ret = 0; // :3

    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxBitmap", "getFontSizeAccordingHeight", "(I)I")) { // :3
        ret = t.env->CallStaticIntMethod(t.classID, t.methodID, height); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3

    return ret; // :3
} // :3

gd::string getStringWithEllipsisJni(const char* pszText, float width, float fontSize) { // :3
    gd::string ret; // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxBitmap", "getStringWithEllipsis", "(Ljava/lang/String;FF)Ljava/lang/String;")) { // :3
        jstring stringArg1; // :3

        if (!pszText) { // :3
            stringArg1 = t.env->NewStringUTF(""); // :3
        } else { // :3
            stringArg1 = t.env->NewStringUTF(pszText); // :3
        } // :3

        jstring retFromJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, stringArg1, width, fontSize); // :3
        const char* str = t.env->GetStringUTFChars(retFromJava, 0); // :3
        ret = str; // :3

        t.env->ReleaseStringUTFChars(retFromJava, str); // :3
        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
    return ret; // :3
} // :3

