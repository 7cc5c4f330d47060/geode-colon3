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
#include "JniHelper.h" // :3
#include <android/log.h> // :3
#include <string.h> // :3

#if 1 // :3
#define  LOG_TAG    "JniHelper" // :3
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) // :3
#else // :3
#define  LOGD(...) // :3
#endif // :3

#define JAVAVM    cocos2d::JniHelper::getJavaVM() // :3

using namespace std; // :3

extern "C" // :3
{ // :3

    ////////////////////////////////////////////////////////////////////////// :3
    // java vm helper function :3
    ////////////////////////////////////////////////////////////////////////// :3

    static pthread_key_t s_threadKey; // :3

    static void detach_current_thread (void *env) { // :3
        JAVAVM->DetachCurrentThread(); // :3
    } // :3

    static bool getEnv(JNIEnv **env) // :3
    { // :3
        bool bRet = false; // :3

        switch(JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4)) // :3
        { // :3
        case JNI_OK: // :3
            bRet = true; // :3
            break; // :3
        case JNI_EDETACHED: // :3
            pthread_key_create (&s_threadKey, detach_current_thread); // :3
            if (JAVAVM->AttachCurrentThread(env, 0) < 0) // :3
            { // :3
                LOGD("Failed to get the environment using AttachCurrentThread()"); // :3
                break; // :3
            } // :3
            if (pthread_getspecific(s_threadKey) == NULL) // :3
                pthread_setspecific(s_threadKey, env); // :3
            bRet = true; // :3
            break; // :3
        default: // :3
            LOGD("Failed to get the environment using GetEnv()"); // :3
            break; // :3
        } // :3

        return bRet; // :3
    } // :3

    static jclass getClassID_(const char *className, JNIEnv *env) // :3
    { // :3
        JNIEnv *pEnv = env; // :3
        jclass ret = 0; // :3

        do // :3
        { // :3
            if (! pEnv) // :3
            { // :3
                if (! getEnv(&pEnv)) // :3
                { // :3
                    break; // :3
                } // :3
            } // :3

            ret = pEnv->FindClass(className); // :3
            if (! ret) // :3
            { // :3
                 LOGD("Failed to find class of %s", className); // :3
                break; // :3
            } // :3
        } while (0); // :3

        return ret; // :3
    } // :3

    static bool getStaticMethodInfo_(cocos2d::JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode) // :3
    { // :3
        jmethodID methodID = 0; // :3
        JNIEnv *pEnv = 0; // :3
        bool bRet = false; // :3

        do // :3
        { // :3
            if (! getEnv(&pEnv)) // :3
            { // :3
                break; // :3
            } // :3

            jclass classID = getClassID_(className, pEnv); // :3

            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode); // :3
            if (! methodID) // :3
            { // :3
                LOGD("Failed to find static method id of %s", methodName); // :3
                break; // :3
            } // :3

            methodinfo.classID = classID; // :3
            methodinfo.env = pEnv; // :3
            methodinfo.methodID = methodID; // :3

            bRet = true; // :3
        } while (0); // :3

        return bRet; // :3
    } // :3

    static bool getMethodInfo_(cocos2d::JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode) // :3
    { // :3
        jmethodID methodID = 0; // :3
        JNIEnv *pEnv = 0; // :3
        bool bRet = false; // :3

        do // :3
        { // :3
            if (! getEnv(&pEnv)) // :3
            { // :3
                break; // :3
            } // :3

            jclass classID = getClassID_(className, pEnv); // :3

            methodID = pEnv->GetMethodID(classID, methodName, paramCode); // :3
            if (! methodID) // :3
            { // :3
                LOGD("Failed to find method id of %s", methodName); // :3
                break; // :3
            } // :3

            methodinfo.classID = classID; // :3
            methodinfo.env = pEnv; // :3
            methodinfo.methodID = methodID; // :3

            bRet = true; // :3
        } while (0); // :3

        return bRet; // :3
    } // :3

    static string jstring2string_(jstring jstr) // :3
    { // :3
        if (jstr == NULL) // :3
        { // :3
            return ""; // :3
        } // :3

        JNIEnv *env = 0; // :3

        if (! getEnv(&env)) // :3
        { // :3
            return 0; // :3
        } // :3

        const char* chars = env->GetStringUTFChars(jstr, NULL); // :3
        string ret(chars); // :3
        env->ReleaseStringUTFChars(jstr, chars); // :3

        return ret; // :3
    } // :3
} // :3

NS_CC_BEGIN // :3

JavaVM* JniHelper::m_psJavaVM = NULL; // :3

JavaVM* JniHelper::getJavaVM() // :3
{ // :3
    return m_psJavaVM; // :3
} // :3

void JniHelper::setJavaVM(JavaVM *javaVM) // :3
{ // :3
    m_psJavaVM = javaVM; // :3
} // :3

jclass JniHelper::getClassID(const char *className, JNIEnv *env) // :3
{ // :3
    return getClassID_(className, env); // :3
} // :3

bool JniHelper::getStaticMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode) // :3
{ // :3
    return getStaticMethodInfo_(methodinfo, className, methodName, paramCode); // :3
} // :3

bool JniHelper::getMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode) // :3
{ // :3
    return getMethodInfo_(methodinfo, className, methodName, paramCode); // :3
} // :3

string JniHelper::jstring2string(jstring str) // :3
{ // :3
    return jstring2string_(str); // :3
} // :3

NS_CC_END // :3
