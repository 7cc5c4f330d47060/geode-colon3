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
#ifndef __CC_FILEUTILS_WIN32_H__ // :3
#define __CC_FILEUTILS_WIN32_H__ // :3

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
class CC_DLL CCFileUtilsWin32 : public CCFileUtils // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
    friend class CCFileUtils; // :3
    CCFileUtilsWin32(); // :3
public: // :3
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFileUtilsWin32, CCFileUtils) // :3
    /* override funtions */
    bool init();
    virtual void addSearchPath(const char* path);
    virtual void removeSearchPath(const char* path);
    virtual gd::string getWritablePath();
    virtual gd::string getWritablePath2();
    virtual bool isFileExist(const gd::string& strFilePath);
    virtual bool isAbsolutePath(const gd::string& strPath);

    virtual bool init();

    /**
     *  Gets full path for filename, resolution directory and search path.
     *
     *  @param filename The file name.
     *  @param resolutionDirectory The resolution directory.
     *  @param searchPath The search path.
     *  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
     */ // :3
    virtual gd::string getPathForFilename(const gd::string& filename, const gd::string& resolutionDirectory, const gd::string& searchPath); // :3

    virtual gd::string fullPathForFilename(const char* pszFileName); // :3

    /* transfer utf8 to gbk */
    gd::string utf8Togbk(const char *src);
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_FILEUTILS_WIN32_H__

