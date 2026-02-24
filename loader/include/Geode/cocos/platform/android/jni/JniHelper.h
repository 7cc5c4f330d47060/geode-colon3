/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

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
#ifndef __ANDROID_JNI_HELPER_H__ // :3
#define __ANDROID_JNI_HELPER_H__ // :3

#include <jni.h> // :3
#include <string> // :3
#include <Geode/c++stl/string.hpp> // :3
#include "../../CCPlatformMacros.h" // :3

NS_CC_BEGIN // :3

typedef struct JniMethodInfo_ // :3
{ // :3
    JNIEnv *    env; // :3
    jclass      classID; // :3
    jmethodID   methodID; // :3
} JniMethodInfo; // :3

class CC_DLL JniHelper // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    static JavaVM* getJavaVM(); // :3
    static void setJavaVM(JavaVM *javaVM); // :3
    static jclass getClassID(const char *className, JNIEnv *env=0); // :3
    static bool getStaticMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode); // :3
    static bool getMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode); // :3
    static gd::string jstring2string(jstring str); // :3

    // @note RobTop Addition :3
    static double getPlatformTimestamp(); // :3

private: // :3
    static JavaVM *m_psJavaVM; // :3
}; // :3

NS_CC_END // :3

#endif // __ANDROID_JNI_HELPER_H__ :3
