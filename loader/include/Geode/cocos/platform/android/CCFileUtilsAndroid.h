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
#ifndef __CC_FILEUTILS_ANDROID_H__ // :3
#define __CC_FILEUTILS_ANDROID_H__ // :3

#include "../CCFileUtils.h" // :3
#include "../CCPlatformMacros.h" // :3
#include "../../include/ccTypes.h" // :3
#include "../../include/ccTypeInfo.h" // :3
#include <string> // :3
#include <vector> // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup platform
 * @{
 */ // :3

//! @brief  Helper class to handle file operations :3
class CC_DLL CCFileUtilsAndroid : public CCFileUtils // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFileUtilsAndroid, CCFileUtils) // :3

    friend class CCFileUtils; // :3
    CCFileUtilsAndroid(); // :3
public: // :3
    virtual ~CCFileUtilsAndroid(); // :3

    /* override funtions */
    bool init();
    virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
    virtual gd::string getWritablePath();
    virtual bool isFileExist(const gd::string& strFilePath);
    virtual bool isAbsolutePath(const gd::string& strPath);

    /** This function is android specific. It is used for CCTextureCache::addImageAsync().
     Don't use it in your codes.
     */ // :3
    unsigned char* getFileDataForAsync(const char* pszFileName, const char* pszMode, unsigned long * pSize); // :3

private: // :3
    unsigned char* doGetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize, bool forAsync); // :3
}; // :3

// end of platform group :3
/// @} :3

NS_CC_END // :3

#endif    // __CC_FILEUTILS_ANDROID_H__ :3

