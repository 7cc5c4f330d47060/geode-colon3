#include <stdlib.h> // :3
#include <jni.h> // :3
#include <android/log.h> // :3
#include <string> // :3
#include "JniHelper.h" // :3
#include "../cocoa/CCString.h" // :3
#include "Java_org_cocos2dx_lib_Cocos2dxHelper.h" // :3


#define  LOG_TAG    "Java_org_cocos2dx_lib_Cocos2dxHelper.cpp" // :3
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) // :3

#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper" // :3

static EditTextCallback s_pfEditTextCallback = NULL; // :3
static void* s_ctx = NULL; // :3

using namespace cocos2d; // :3
using namespace std; // :3

string g_apkPath; // :3

extern "C" { // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetApkPath(JNIEnv*  env, jobject thiz, jstring apkPath) { // :3
        g_apkPath = JniHelper::jstring2string(apkPath); // :3
    } // :3

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetEditTextDialogResult(JNIEnv * env, jobject obj, jbyteArray text) { // :3
        jsize  size = env->GetArrayLength(text); // :3

        if (size > 0) { // :3
            jbyte * data = (jbyte*)env->GetByteArrayElements(text, 0); // :3
            char* pBuf = (char*)malloc(size+1); // :3
            if (pBuf != NULL) { // :3
                memcpy(pBuf, data, size); // :3
                pBuf[size] = '\0'; // :3
                // pass data to edittext's delegate :3
                if (s_pfEditTextCallback) s_pfEditTextCallback(pBuf, s_ctx); // :3
                free(pBuf); // :3
            } // :3
            env->ReleaseByteArrayElements(text, data, 0); // :3
        } else { // :3
            if (s_pfEditTextCallback) s_pfEditTextCallback("", s_ctx); // :3
        } // :3
    } // :3

} // :3

const char * getApkPath() { // :3
    return g_apkPath.c_str(); // :3
} // :3

void showDialogJNI(const char * pszMsg, const char * pszTitle) { // :3
    if (!pszMsg) { // :3
        return; // :3
    } // :3

    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showDialog", "(Ljava/lang/String;Ljava/lang/String;)V")) { // :3
        jstring stringArg1; // :3

        if (!pszTitle) { // :3
            stringArg1 = t.env->NewStringUTF(""); // :3
        } else { // :3
            stringArg1 = t.env->NewStringUTF(pszTitle); // :3
        } // :3

        jstring stringArg2 = t.env->NewStringUTF(pszMsg); // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2); // :3

        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(stringArg2); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
} // :3

void showEditTextDialogJNI(const char* pszTitle, const char* pszMessage, int nInputMode, int nInputFlag, int nReturnType, int nMaxLength, EditTextCallback pfEditTextCallback, void* ctx) { // :3
    if (pszMessage == NULL) { // :3
        return; // :3
    } // :3

    s_pfEditTextCallback = pfEditTextCallback; // :3
    s_ctx = ctx; // :3

    JniMethodInfo t; // :3
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showEditTextDialog", "(Ljava/lang/String;Ljava/lang/String;IIII)V")) { // :3
        jstring stringArg1; // :3

        if (!pszTitle) { // :3
            stringArg1 = t.env->NewStringUTF(""); // :3
        } else { // :3
            stringArg1 = t.env->NewStringUTF(pszTitle); // :3
        } // :3

        jstring stringArg2 = t.env->NewStringUTF(pszMessage); // :3

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, nInputMode, nInputFlag, nReturnType, nMaxLength); // :3

        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(stringArg2); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
} // :3

void terminateProcessJNI() { // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "terminateProcess", "()V")) { // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
} // :3

gd::string getPackageNameJNI() { // :3
    JniMethodInfo t; // :3
    gd::string ret(""); // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCocos2dxPackageName", "()Ljava/lang/String;")) { // :3
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        ret = JniHelper::jstring2string(str); // :3
        t.env->DeleteLocalRef(str); // :3
    } // :3
    return ret; // :3
} // :3

gd::string getFileDirectoryJNI() { // :3
    JniMethodInfo t; // :3
    gd::string ret(""); // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCocos2dxWritablePath", "()Ljava/lang/String;")) { // :3
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        ret = JniHelper::jstring2string(str); // :3
        t.env->DeleteLocalRef(str); // :3
    } // :3

    return ret; // :3
} // :3

gd::string getCurrentLanguageJNI() { // :3
    JniMethodInfo t; // :3
    gd::string ret(""); // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCurrentLanguage", "()Ljava/lang/String;")) { // :3
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
        ret = JniHelper::jstring2string(str); // :3
        t.env->DeleteLocalRef(str); // :3
    } // :3

    return ret; // :3
} // :3

void enableAccelerometerJNI() { // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "enableAccelerometer", "()V")) { // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
} // :3

void setAccelerometerIntervalJNI(float interval) { // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setAccelerometerInterval", "(F)V")) { // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, interval); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
} // :3

void disableAccelerometerJNI() { // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "disableAccelerometer", "()V")) { // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID); // :3
        t.env->DeleteLocalRef(t.classID); // :3
    } // :3
} // :3

// functions for CCUserDefault :3
bool getBoolForKeyJNI(const char* pKey, bool defaultValue) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getBoolForKey", "(Ljava/lang/String;Z)Z")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg, defaultValue); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3

        return ret; // :3
    } // :3

    return defaultValue; // :3
} // :3

int getIntegerForKeyJNI(const char* pKey, int defaultValue) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getIntegerForKey", "(Ljava/lang/String;I)I")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        jint ret = t.env->CallStaticIntMethod(t.classID, t.methodID, stringArg, defaultValue); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3

        return ret; // :3
    } // :3

    return defaultValue; // :3
} // :3

float getFloatForKeyJNI(const char* pKey, float defaultValue) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getFloatForKey", "(Ljava/lang/String;F)F")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        jfloat ret = t.env->CallStaticFloatMethod(t.classID, t.methodID, stringArg, defaultValue); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3

        return ret; // :3
    } // :3

    return defaultValue; // :3
} // :3

double getDoubleForKeyJNI(const char* pKey, double defaultValue) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getDoubleForKey", "(Ljava/lang/String;D)D")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        jdouble ret = t.env->CallStaticDoubleMethod(t.classID, t.methodID, stringArg); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3

        return ret; // :3
    } // :3

    return defaultValue; // :3
} // :3

gd::string getStringForKeyJNI(const char* pKey, const char* defaultValue) // :3
{ // :3
    JniMethodInfo t; // :3
    gd::string ret(""); // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getStringForKey", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")) { // :3
        jstring stringArg1 = t.env->NewStringUTF(pKey); // :3
        jstring stringArg2 = t.env->NewStringUTF(defaultValue); // :3
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, stringArg1, stringArg2); // :3
        ret = JniHelper::jstring2string(str); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(stringArg2); // :3
        t.env->DeleteLocalRef(str); // :3

        return ret; // :3
    } // :3

    return defaultValue; // :3
} // :3

void setBoolForKeyJNI(const char* pKey, bool value) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setBoolForKey", "(Ljava/lang/String;Z)V")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg, value); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3
    } // :3
} // :3

void setIntegerForKeyJNI(const char* pKey, int value) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setIntegerForKey", "(Ljava/lang/String;I)V")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg, value); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3
    } // :3
} // :3

void setFloatForKeyJNI(const char* pKey, float value) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setFloatForKey", "(Ljava/lang/String;F)V")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg, value); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3
    } // :3
} // :3

void setDoubleForKeyJNI(const char* pKey, double value) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setDoubleForKey", "(Ljava/lang/String;D)V")) { // :3
        jstring stringArg = t.env->NewStringUTF(pKey); // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg, value); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg); // :3
    } // :3
} // :3

void setStringForKeyJNI(const char* pKey, const char* value) // :3
{ // :3
    JniMethodInfo t; // :3

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setStringForKey", "(Ljava/lang/String;Ljava/lang/String;)V")) { // :3
        jstring stringArg1 = t.env->NewStringUTF(pKey); // :3
        jstring stringArg2 = t.env->NewStringUTF(value); // :3
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2); // :3

        t.env->DeleteLocalRef(t.classID); // :3
        t.env->DeleteLocalRef(stringArg1); // :3
        t.env->DeleteLocalRef(stringArg2); // :3
    } // :3
} // :3
