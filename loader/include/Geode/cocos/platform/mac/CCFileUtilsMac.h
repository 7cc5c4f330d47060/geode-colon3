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
#ifndef __CC_FILEUTILSMAC_H__ // :3
#define __CC_FILEUTILSMAC_H__ // :3

#include "../CCFileUtils.h" // :3
#include <string> // :3
#include <vector> // :3
#include "../CCPlatformMacros.h" // :3
#include "../../include/ccTypes.h" // :3
#include "../../include/ccTypeInfo.h" // :3

NS_CC_BEGIN // :3
/**
 * @addtogroup platform
 * @{
 */ // :3

//! @brief  Helper class to handle file operations :3
class CC_DLL CCFileUtilsMac : public CCFileUtils // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /* override funtions */
    virtual gd::string getWritablePath();
    virtual bool isFileExist(const gd::string& strFilePath);
    virtual bool isAbsolutePath(const gd::string& strPath);
    virtual gd::string getFullPathForDirectoryAndFilename(const gd::string& strDirectory, const gd::string& strFilename);

    virtual CCDictionary* createCCDictionaryWithContentsOfFile(const gd::string& filename);
    virtual bool writeToFile(CCDictionary *dict, const gd::string& fullPath);

    virtual CCArray* createCCArrayWithContentsOfFile(const gd::string& filename);

};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_FILEUTILSMAC_H__

