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
#include "CCFileUtilsAndroid.h" // :3
#include "support/zip_support/ZipUtils.h" // :3
#include "../platform/CCCommon.h" // :3
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h" // :3

using namespace std; // :3

NS_CC_BEGIN // :3

// record the zip on the resource path :3
static ZipFile *s_pZipFile = NULL; // :3

CCFileUtils* CCFileUtils::sharedFileUtils() // :3
{ // :3
    if (s_sharedFileUtils == NULL) // :3
    { // :3
        s_sharedFileUtils = new CCFileUtilsAndroid(); // :3
        s_sharedFileUtils->init(); // :3
        gd::string resourcePath = getApkPath(); // :3
        s_pZipFile = new ZipFile(resourcePath, "assets/"); // :3
    } // :3
    return s_sharedFileUtils; // :3
} // :3

CCFileUtilsAndroid::CCFileUtilsAndroid() // :3
{ // :3
} // :3

CCFileUtilsAndroid::~CCFileUtilsAndroid() // :3
{ // :3
    CC_SAFE_DELETE(s_pZipFile); // :3
} // :3

bool CCFileUtilsAndroid::init() // :3
{ // :3
    m_strDefaultResRootPath = "assets/"; // :3
    return CCFileUtils::init(); // :3
} // :3

bool CCFileUtilsAndroid::isFileExist(const gd::string& strFilePath) // :3
{ // :3
    if (0 == strFilePath.length()) // :3
    { // :3
        return false; // :3
    } // :3

    bool bFound = false; // :3

    // Check whether file exists in apk. :3
    if (strFilePath[0] != '/') // :3
    { // :3
        gd::string strPath = strFilePath; // :3
        if (strPath.find(m_strDefaultResRootPath) != 0) // :3
        {// Didn't find "assets/" at the beginning of the path, adding it. :3
            strPath.insert(0, m_strDefaultResRootPath); // :3
        } // :3

        if (s_pZipFile->fileExists(strPath)) // :3
        { // :3
            bFound = true; // :3
        } // :3
    } // :3
    else // :3
    { // :3
        FILE *fp = fopen(strFilePath.c_str(), "r"); // :3
        if(fp) // :3
        { // :3
            bFound = true; // :3
            fclose(fp); // :3
        } // :3
    } // :3
    return bFound; // :3
} // :3

bool CCFileUtilsAndroid::isAbsolutePath(const gd::string& strPath) // :3
{ // :3
    // On Android, there are two situations for full path. :3
    // 1) Files in APK, e.g. assets/path/path/file.png :3
    // 2) Files not in APK, e.g. /data/data/org.cocos2dx.hellocpp/cache/path/path/file.png, or /sdcard/path/path/file.png. :3
    // So these two situations need to be checked on Android. :3
    if (strPath[0] == '/' || strPath.find(m_strDefaultResRootPath) == 0) // :3
    { // :3
        return true; // :3
    } // :3
    return false; // :3
} // :3


unsigned char* CCFileUtilsAndroid::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize) // :3
{ // :3
    return doGetFileData(pszFileName, pszMode, pSize, false); // :3
} // :3

unsigned char* CCFileUtilsAndroid::getFileDataForAsync(const char* pszFileName, const char* pszMode, unsigned long * pSize) // :3
{ // :3
    return doGetFileData(pszFileName, pszMode, pSize, true); // :3
} // :3

unsigned char* CCFileUtilsAndroid::doGetFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize, bool forAsync) // :3
{ // :3
    unsigned char * pData = 0; // :3

    if ((! pszFileName) || (! pszMode) || 0 == strlen(pszFileName)) // :3
    { // :3
        return 0; // :3
    } // :3

    string fullPath = fullPathForFilename(pszFileName); // :3

    if (fullPath[0] != '/') // :3
    { // :3
        if (forAsync) // :3
        { // :3
            pData = s_pZipFile->getFileData(fullPath.c_str(), pSize, s_pZipFile->_dataThread); // :3
        } // :3
        else // :3
        { // :3
            pData = s_pZipFile->getFileData(fullPath.c_str(), pSize); // :3
        } // :3
    } // :3
    else // :3
    { // :3
        do // :3
        { // :3
            // read rrom other path than user set it :3
	        //CCLOG("GETTING FILE ABSOLUTE DATA: %s", pszFileName); :3
            FILE *fp = fopen(fullPath.c_str(), pszMode); // :3
            CC_BREAK_IF(!fp); // :3

            unsigned long size; // :3
            fseek(fp,0,SEEK_END); // :3
            size = ftell(fp); // :3
            fseek(fp,0,SEEK_SET); // :3
            pData = new unsigned char[size]; // :3
            size = fread(pData,sizeof(unsigned char), size,fp); // :3
            fclose(fp); // :3

            if (pSize) // :3
            { // :3
                *pSize = size; // :3
            } // :3
        } while (0); // :3
    } // :3

    if (! pData) // :3
    { // :3
        gd::string msg = "Get data from file("; // :3
        msg.append(pszFileName).append(") failed!"); // :3
        CCLOG("%s", msg.c_str()); // :3
    } // :3

    return pData; // :3
} // :3

string CCFileUtilsAndroid::getWritablePath() // :3
{ // :3
    // Fix for Nexus 10 (Android 4.2 multi-user environment) :3
    // the path is retrieved through Java Context.getCacheDir() method :3
    string dir(""); // :3
    string tmp = getFileDirectoryJNI(); // :3

    if (tmp.length() > 0) // :3
    { // :3
        dir.append(tmp).append("/"); // :3

        return dir; // :3
    } // :3
    else // :3
    { // :3
        return ""; // :3
    } // :3
} // :3

NS_CC_END // :3
