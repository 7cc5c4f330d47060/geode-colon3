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

#include "../platform/CCCommon.h" // :3
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h" // :3
#include <android/log.h> // :3
#include <stdio.h> // :3
#include <jni.h> // :3

NS_CC_BEGIN // :3

#define MAX_LEN         (cocos2d::kMaxLogLen + 1) // :3

void CCLog(const char * pszFormat, ...) // :3
{ // :3
    char buf[MAX_LEN]; // :3

    va_list args; // :3
    va_start(args, pszFormat); // :3
    vsnprintf(buf, MAX_LEN, pszFormat, args); // :3
    va_end(args); // :3

    __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info", "%s", buf); // :3
} // :3

void CCMessageBox(const char * pszMsg, const char * pszTitle) // :3
{ // :3
    showDialogJNI(pszMsg, pszTitle); // :3
} // :3

void CCLuaLog(const char * pszFormat) // :3
{ // :3
    __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info", "%s", pszFormat); // :3
} // :3

NS_CC_END // :3
